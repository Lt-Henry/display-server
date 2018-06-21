

#include "server.hpp"

#include <fcntl.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <sys/kd.h>
#include <unistd.h>

#include <iostream>
#include <stdexcept>
#include <chrono>

using namespace ds;
using namespace ds::drm;
using namespace std;

int open_restricted(const char *path, int flags, void *user_data)
{
    clog<<"open "<<path<<endl;
    return open(path,flags);
}

void close_restricted(int fd, void *user_data)
{
    clog<<"close "<<fd<<endl;
    close(fd);
}


Server::Server()
{

    quit_request=0;

    pointer_x=0;
    pointer_y=0;

    udev = udev_new();
    
    if (!udev) {
        throw runtime_error("Failed to create udev object!");
    }
    
    struct udev_enumerate *enumerate;
    enumerate = udev_enumerate_new(udev);
    udev_enumerate_add_match_subsystem(enumerate, "drm");
    udev_enumerate_add_match_property(enumerate,"DEVTYPE","drm_minor");
    
    struct udev_list_entry *devices, *dev_list_entry;
    
    udev_enumerate_scan_devices(enumerate);
    devices = udev_enumerate_get_list_entry(enumerate);
    
    clog<<"* Available gpu:"<<endl;
    udev_list_entry_foreach(dev_list_entry, devices) {
        const char* path = udev_list_entry_get_name(dev_list_entry);
        
        struct udev_device* dev = udev_device_new_from_syspath(udev,path);
        
        string devnode = udev_device_get_devnode(dev);
        clog<<"    -"<<devnode<<endl;
        
        drm_devices.push_back(devnode);
       
       udev_device_unref(dev);
    }
    
    udev_enumerate_unref(enumerate);
}

Server::~Server()
{
    
}

void Server::init(string seat)
{
    clog<<"* init"<<endl;
    clog<<"* loading libinput stack"<<endl;
    
    interface.open_restricted=open_restricted;
    interface.close_restricted=close_restricted;
    
    libinput = libinput_udev_create_context(&interface,nullptr,udev);
    
    if (libinput_udev_assign_seat(libinput,seat.c_str())!=0) {
        throw runtime_error("Failed to assign seat");
    }
    
    clog<<"* looking for a valid modeset"<<endl;
    clog<<"* "<<drm_devices[0]<<endl;
    
    gpu = new Gpu(drm_devices[0]);
    gpu->update();
    
    if (gpu->support_dumb_buffer()) {
        clog<<"* dumb buffer supported"<<endl;
    }
    
    if (gpu->support_vsync()) {
        clog<<"* vsync supported"<<endl;
    }
    
    vector<Connector> connectors = gpu->get_connectors();
    
    
    
    for (Connector& c:connectors) {
        if (c.is_connected()) {
            connector=c;
            break;
        }
    }
    
    if (!connector.is_connected()) {
        throw runtime_error("There is no connector");
    }
    
    vector<struct drm_mode_modeinfo> modes = connector.get_modes();
    
    if (modes.size()==0) {
        throw runtime_error("No valid modes for connector");
    }
    
    width=modes[0].hdisplay;
    height=modes[0].vdisplay;
    
    dumbs[0] = new DumbBuffer(*gpu,width,height);
    dumbs[1] = new DumbBuffer(*gpu,width,height);
    
    buffer_id=0;
    buffer=dumbs[buffer_id];
    
    Encoder encoder;
    Crtc crtc;
    
    encoder = connector.get_encoder();
    crtc = encoder.get_crtc();
    
    
    
    clog<<"* toggling to graphic tty"<<endl;
    
    //set_graphic_tty();
}

void Server::run()
{
    
    struct pollfd fds;
    
    fds.fd = libinput_get_fd(libinput);
    fds.events = POLLIN;
    fds.revents = 0;
    
    Encoder encoder;
    Crtc crtc;
    
    encoder = connector.get_encoder();
    crtc = encoder.get_crtc();
    crtc.add_fb(connector,*buffer);
    
    
    
    Surface pointer("dspointer.png");
    Surface background("dsback.png");
    
    auto t0 = std::chrono::steady_clock::now();
    int fps=0;
    
    set_graphic_tty();

    while(!quit_request) {
        poll(&fds,1,1);
        
        libinput_dispatch(libinput);
        
        struct libinput_event* event;
        
        event = libinput_get_event(libinput);
        
        int32_t key;
        
        if (event) {
            
            switch (libinput_event_get_type(event)) {
            
                case LIBINPUT_EVENT_POINTER_MOTION_ABSOLUTE:
                    pointer_x=libinput_event_pointer_get_absolute_x_transformed((libinput_event_pointer*)event,width);
                    pointer_y=libinput_event_pointer_get_absolute_y_transformed((libinput_event_pointer*)event,height);
                    
                    clog<<"absolute "<<pointer_x<<","<<pointer_y<<endl;
                break;
                
                case LIBINPUT_EVENT_POINTER_MOTION:
                    pointer_x+=libinput_event_pointer_get_dx((libinput_event_pointer*)event);
                    pointer_y+=libinput_event_pointer_get_dy((libinput_event_pointer*)event);
                    
                    clog<<"relative "<<pointer_x<<","<<pointer_y<<endl;
                break;
                
                case LIBINPUT_EVENT_KEYBOARD_KEY:
                    key=libinput_event_keyboard_get_key((libinput_event_keyboard*)event);

                    clog<<"key "<<key<<endl;
                    
                    if (key==1) {
                        quit_request=true;
                    }
                break;
                
                default:
                
                break;
            }
            
            
            libinput_event_destroy(event);
        }
        
        //buffer->fill(0xffffffff);
        buffer->blit(background,0,0);
        
        buffer->blit(pointer,pointer_x,pointer_y,1);
        
        buffer->dirty();
        
        crtc.page_flip(*buffer);
        
        buffer_id++;
        buffer_id=buffer_id%2;
        buffer=dumbs[buffer_id];
        
        fps++;
        auto t1 = std::chrono::steady_clock::now();
        
        double delta=std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count();
        
        if (delta>=1000) {
            clog<<"fps: "<<fps<<endl;
            fps=0;
            t0=std::chrono::steady_clock::now();
        }
        //crtc.vsync();
        
        //usleep(16000);
    }

    clog<<"* out of main loop"<<endl;
}

void Server::destroy()
{
    clog<<"back to text tty"<<endl;
    set_text_tty();
}

void Server::set_graphic_tty()
{
    int tty_fd = open("/dev/tty", O_RDWR);
    ioctl(tty_fd,KDSETMODE,KD_GRAPHICS);
    close(tty_fd);
}

void Server::set_text_tty()
{
    int tty_fd = open("/dev/tty", O_RDWR);
    ioctl(tty_fd,KDSETMODE,KD_TEXT);
    close(tty_fd);
}

void Server::flip()
{
    
}
