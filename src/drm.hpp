

#ifndef DS_DRM
#define DS_DRM

#include <map>
#include <string>

namespace ds
{
    class Drm
    {
        public:
        
        int fd;
        
        std::map<int,std::string> connector_type;
        std::map<int,std::string> connector_status;
        
        Drm(const char* path);
        ~Drm();
    
    };
}

#endif
