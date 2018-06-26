
#ifndef DS_ENCODER
#define DS_ENCODER

#include "drmobject.hpp"
#include "crtc.hpp"

#include <cstdint>
#include <string>

namespace ds
{
    namespace drm
    {
        class Encoder : public Object
        {
            public:
            
            uint32_t id;
            uint32_t type;
            
            uint32_t crtc_id;
            
            Encoder();
            Encoder(int fd,uint32_t id);
            
            void update();
            
            std::string get_type_name();
            
            Crtc get_crtc();
            
        };
    }
}


#endif
