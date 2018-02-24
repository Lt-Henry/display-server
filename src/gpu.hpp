

#ifndef DS_GPU
#define DS_GPU

#include <map>
#include <string>
#include <vector>

#include "connector.hpp"

namespace ds
{
    namespace drm
    {
        class Gpu
        {
            public:
            
            static std::vector<std::string> get_cards();
            
            int fd;
            
            std::map<int,std::string> connector_type;
            std::map<int,std::string> connector_status;
            
            Gpu(std::string path);
            ~Gpu();
            
            /*!
                Get a list of connectors
            */
            std::vector<Connector> get_connectors();
            
            /*!
                Become master
            */
            void set_master();
            
            /*!
                Drop master
            */
            void drop_master();
        
        };
    }
}

#endif
