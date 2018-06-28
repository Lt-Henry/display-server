

#ifndef DS_SURFACE
#define DS_SURFACE

#include <cstdint>

namespace ds
{
    class Surface
    {
        public:
        
        /*! width in pixels */
        uint16_t width;
        
        /*! height in pixels */
        uint16_t height;
        
        /*! bytes per pixel */
        uint8_t bpp;
        
        /*! pitch in bytes */
        uint32_t pitch;
        
        /*! byte data */
        uint8_t* data;
        
        /*!
            Default constructor, does not create the surface
        */
        Surface();
        
        /*!
            Creates a surface with given size
        */
        Surface(uint16_t width,uint16_t height);
        
        
        Surface(const char* path);
        
        /*!
            Default destructor, surface data is not freed here
        */
        virtual ~Surface();
        
        /*!
            Destroys surface content
        */
        virtual void destroy();
        
        /*!
            Fills surface with given pixel
        */
        void fill(uint32_t pixel);
        
        /*!
            Blits another surface at x,y coordinates
        */
        void blit(Surface& src,int x,int y,int flags=0);
        
        /*!
            Check if data is not corrupted
        */
        bool is_good();
    };
}

#endif
