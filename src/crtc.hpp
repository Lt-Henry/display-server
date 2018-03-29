
#ifndef DS_CRTC
#define DS_CRTC

#include "dumb.hpp"

#include <cstdint>

namespace ds
{
    namespace drm
    {
        class Connector;
        
        class Crtc
        {
            public:
            
            int fd;
            uint32_t id;
            
            Crtc();
            Crtc(int fd,uint32_t id);
            
            void update();
            
            void add_fb(Connector& conn,DumbBuffer& fb);
            
            void page_flip(DumbBuffer& fb);
            
            void vsync();
        };
    }
}

#endif
