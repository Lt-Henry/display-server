
#ifndef DS_CONNECTOR
#define DS_CONNECTOR

#include <cstdint>
#include <vector>

#include <libdrm/drm_mode.h>

#include "encoder.hpp"

namespace ds
{
    namespace drm
    {
        class Connector
        {
            public:
            
            int fd;
            uint32_t id;
            
            uint32_t type;
            uint32_t connection;
            uint32_t encoder_id;
            
            std::vector<uint32_t> encoder_ids;
            std::vector<struct drm_mode_modeinfo> modes;
            std::vector<uint32_t> prop_ids;
            std::vector<uint32_t> prop_value_ids;
            
            
            Connector();
            Connector(int fd,uint32_t id);
            ~Connector();
            
            void update();
            
            bool is_connected();
            
            std::string get_type_name();
            
            void get_modes();
            
            std::vector<Encoder> get_encoders();
        };
    }
}

#endif
