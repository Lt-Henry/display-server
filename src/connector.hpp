
#ifndef DS_CONNECTOR
#define DS_CONNECTOR

#include <cstdint>

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
            uint32_t encoders;
            uint32_t modes;
            
            Connector();
            Connector(int fd,uint32_t id);
            ~Connector();
            
            void update();
        };
    }
}

#endif
