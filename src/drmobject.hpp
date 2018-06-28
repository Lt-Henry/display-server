
#ifndef DS_DRM_OBJECT
#define DS_DRM_OBJECT

namespace ds
{
    namespace drm
    {
        class Object
        {
            protected:
            
            int fd;
            
            Object();
            Object(int fd);
            virtual ~Object();
            
            virtual void update()=0;
            
            public:
            
            virtual bool is_valid();
            int get_fd();
            
        };
    }
}

#endif
