

#ifndef DS_SURFACE
#define DS_SURFACE

#include <cstdint>

namespace ds
{
    class Surface
    {
        public:
        
        uint32_t width;
        uint32_t height;
        uint32_t bpp;
        uint32_t pitch;
        void* data;
        
        Surface();
        virtual ~Surface();
    };
}

#endif
