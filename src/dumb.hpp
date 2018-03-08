
#ifndef DS_DUMB
#define DS_DUMB

#include "surface.hpp"

namespace ds
{
    namespace drm
    {
    
        
        class Gpu;
        
        class DumbBuffer : public Surface
        {
        
            public:
            
            int fd;
            uint32_t handle;
            uint32_t id;
            
            DumbBuffer();
            
            static DumbBuffer create(Gpu& gpu,int width,int height);
            
            void destroy();
        };
        
    }
}


#endif
