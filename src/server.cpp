

#include <fcntl.h>
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

    while(true) {
    
        libinput_dispatch(libinput);
        
        struct libinput_event* event;
        
        event = libinput_get_event(libinput);
        
        if (event) {
            
            clog<<"+ "<<libinput_event_get_type(event)<<endl;
            
            libinput_event_destroy(event);
        }
    }
/*
    struct udev_enumerate *enumerate;
    struct udev_list_entry *devices, *dev_list_entry;
    struct udev_device *dev;
    
    enumerate = udev_enumerate_new(udev);
    udev_enumerate_add_match_subsystem(enumerate, "input");
    udev_enumerate_scan_devices(enumerate);
    devices = udev_enumerate_get_list_entry(enumerate);
    
    udev_list_entry_foreach(dev_list_entry, devices) {
        const char* path;
        path = udev_list_entry_get_name(dev_list_entry);
        
        clog<<"* "<<path<<endl;
    }
    
    */
}
