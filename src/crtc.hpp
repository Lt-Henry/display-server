
#ifndef DS_CRTC
#define DS_CRTC

#include <cstdint>

namespace ds
{
    namespace drm
    {
        class Crtc
        {
            public:
            
            int fd;
            uint32_t id;
            
            Crtc();
            Crtc(int fd,uint32_t id);
            
            void update();
            
            
        };
    }
}

#endif
