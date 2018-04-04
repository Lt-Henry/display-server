

#include <fcntl.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <iostream>

#include "server.hpp"

using namespace ds;
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

    pointer_x=0;
    pointer_y=0;

    udev = udev_new();
    
    if (!udev) {
        cerr<<"Failed to create udev object!"<<endl;
    }
    
    
    interface.open_restricted=open_restricted;
    interface.close_restricted=close_restricted;
    
    libinput = libinput_udev_create_context(&interface,nullptr,udev);
    
    if(libinput_udev_assign_seat(libinput,"seat0")!=0) {
        cerr<<"Failed to assign seat"<<endl;
    }
    
    
    
}

Server::~Server()
{
    
}

void Server::run()
{

    
    struct pollfd fds;
    
    fds.fd = libinput_get_fd(libinput);
    fds.events = POLLIN;
    fds.revents = 0;

    while(true) {
        poll(&fds,1,1);
        
        libinput_dispatch(libinput);
        
        struct libinput_event* event;
        
        event = libinput_get_event(libinput);
        
        if (event) {
            
            switch (libinput_event_get_type(event)) {
            
                case LIBINPUT_EVENT_POINTER_MOTION_ABSOLUTE:
                    pointer_x=libinput_event_pointer_get_absolute_x((libinput_event_pointer*)event);
                    pointer_y=libinput_event_pointer_get_absolute_y((libinput_event_pointer*)event);
                    
                    clog<<"pointer "<<pointer_x<<","<<pointer_y<<endl;
                break;
                
                case LIBINPUT_EVENT_POINTER_MOTION:
                    pointer_x+=libinput_event_pointer_get_dx((libinput_event_pointer*)event);
                    pointer_y+=libinput_event_pointer_get_dy((libinput_event_pointer*)event);
                    
                    clog<<"pointer "<<pointer_x<<","<<pointer_y<<endl;
                break;
                
                default:
                
                break;
            }
            
            
            libinput_event_destroy(event);
        }
    }

}
