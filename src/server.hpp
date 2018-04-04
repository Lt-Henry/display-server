
#ifndef DS_SERVER
#define DS_SERVER

#include <libudev.h>
#include <libinput.h>

namespace ds
{
    class Server
    {
    
        public:
        
        // hack hack hack
        int pointer_x,pointer_y;
        
        struct udev* udev;
        struct libinput* libinput;
        struct libinput_interface interface;
        
        Server();
        ~Server();
        
        void run();
    };
}

#endif
