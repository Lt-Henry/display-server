
#ifndef DS_GPU
#define DS_GPU

#include "drmobject.hpp"
#include "connector.hpp"

#include <map>
#include <string>
#include <vector>


namespace ds
{
    namespace drm
    {
        class Gpu : public Object
        {
        
            private:
            
            uint64_t get_capability(uint64_t cid);
        
            public:
            
            
            std::vector<uint32_t> fb_ids;
            std::vector<uint32_t> crtc_ids;
            std::vector<uint32_t> encoder_ids;
            std::vector<uint32_t> connector_ids;
            
            Gpu();
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
            
            /*!
                Update resource counters
            */
            void update();
            
            bool support_dumb_buffer();
            
            bool support_vsync();
        
        };
    }
}

#endif
