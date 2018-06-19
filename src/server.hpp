
#ifndef DS_SERVER
#define DS_SERVER

#include "gpu.hpp"
#include "connector.hpp"
#include "dumb.hpp"

#include <libudev.h>
#include <libinput.h>

#include <string>
#include <vector>

namespace ds
{
    class Server
    {
    
        public:
        
        // hack hack hack
        int pointer_x,pointer_y;
        
        //hack hack hack
        int width,height;
        
        bool quit_request;
        
        struct udev* udev;
        struct libinput* libinput;
        struct libinput_interface interface;
        
        std::vector<std::string> drm_devices;
        
        drm::Gpu* gpu;
        drm::Connector* connector;
        
        drm::DumbBuffer* dumbs[2];
        drm::DumbBuffer* buffer;
        int buffer_id;
        
        Server();
        ~Server();
        
        void init(std::string seat);
        void run();
        void destroy();
        
        void set_graphic_tty();
        void set_text_tty();
        
        void flip();
    };
}

#endif
