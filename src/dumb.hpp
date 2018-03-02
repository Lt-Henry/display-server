
#ifndef DS_DUMB
#define DS_DUMB

#include "surface.hpp"

namespace ds
{
    namespace drm
    {
        class DumbBuffer : public Surface
        {
        
            int fd;
            uint32_t handle;
            
            DumbBuffer();
            
            static DumbBuffer create(int fd);
        };
        
    }
}


#endif
