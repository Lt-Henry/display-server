
#ifndef DS_CONNECTOR
#define DS_CONNECTOR

#include "drmobject.hpp"
#include "encoder.hpp"

#include <libdrm/drm_mode.h>

#include <cstdint>
#include <vector>

namespace ds
{
    namespace drm
    {
        class Connector : public Object
        {
            public:
            
            uint32_t id;
            
            uint32_t type;
            uint32_t connection;
            uint32_t encoder_id;
            
            std::vector<uint32_t> encoder_ids;
            std::vector<struct drm_mode_modeinfo> modes;
            std::vector<uint32_t> prop_ids;
            std::vector<uint64_t> prop_value_ids;
            
            
            Connector();
            Connector(int fd,uint32_t id);
            ~Connector();
            
            void update();
            
            bool is_connected();
            
            std::string get_type_name();
            
            std::vector<struct drm_mode_modeinfo> get_modes();
            
            std::vector<Encoder> get_encoders();
            
            Encoder get_encoder();
        };
    }
}

#endif
