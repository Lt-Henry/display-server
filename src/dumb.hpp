
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
            uint32_t size;
            
            DumbBuffer();
            
            DumbBuffer(Gpu& gpu,int width,int height);
            
            void destroy();
            
            void dirty();
        };
        
    }
}


#endif
