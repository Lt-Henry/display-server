
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
        
        bool quit_request;
        
        struct udev* udev;
        struct libinput* libinput;
        struct libinput_interface interface;
        
        Server();
        ~Server();
        
        void run();
        
        void set_graphic_tty();
        void set_text_tty();
    };
}

#endif
