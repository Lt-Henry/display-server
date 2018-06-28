
#include "surface.hpp"

#include <cmath>
#include <iostream>

#include <png++/png.hpp>

using namespace std;
using namespace ds;

Surface::Surface()
{
    width=0;
    height=0;
    bpp=0;
    pitch=0;
    data=nullptr;
}

Surface::Surface(uint16_t width,uint16_t height)
{
    this->width=width;
    this->height=height;
    
    bpp = 4;
    pitch = width*bpp;
    
    data = new uint8_t[width*height*bpp];
}

Surface::Surface(const char* path)
{
    clog<<"loading "<<path<<endl;
    
    png::image<png::rgba_pixel> image(path);
    
    width = image.get_width();
    height = image.get_height();
    
    clog<<"size "<<width<<"x"<<height<<endl;
    
    bpp = 4;
    pitch = width*bpp;
    
    data = new uint8_t[width*height*bpp];
    
    for (int j=0;j<height;j++) {
        for (int i=0;i<width;i++) {
            png::rgba_pixel pixel = image.get_pixel(i,j);
            
            uint8_t* base = data+(i*bpp+j*pitch);
            base[0] = pixel.blue;
            base[1] = pixel.green;
            base[2] = pixel.red;
            base[3] = pixel.alpha;
        }
    }
}

Surface::~Surface()
{
    destroy();
}

void Surface::destroy()
{
    if (data!=nullptr) {
        delete [] data;
    
        data = nullptr;
    }
}

void Surface::fill(uint32_t pixel)
{
    
    for (int j=0;j<height;j++) {
    
        uint8_t* base = data+(j*pitch);
        uint32_t* pixmap =(uint32_t*)base;

        for (int i=0;i<width;i++) {
            pixmap[i]=pixel;
        }
    }
}

void Surface::blit(Surface& src,int x,int y,int flags)
{
    if (x>=width or y>=height) {
        return;
    }
    
    if ((x+src.width)<0 or (y+src.height)<0) {
        return;
    }
    
    int sx,sy,dx,dy;
    
    sx=0;
    sy=0;
    dx=x;
    dy=y;
    
    if (x<0) {
        sx=-x;
        dx=0;
    }
    
    if (y<0) {
        sy=-y;
        dy=0;
    }
    
    int w,h;
    
    if ((x+src.width)<width) {
        w=src.width;
    }
    else {
        w=src.width-(x+src.width-width);
    }
    
    if ((y+src.height)<height) {
        h=src.height;
    }
    else {
        h=src.height-(y+src.height-height);
    }
    
    int j,jj;
    int i,ii;
    
    if (flags==0) {
    
        for (j=sy,jj=dy;j<h;j++,jj++) {
            uint8_t* source = src.data+(src.pitch*j);
            uint8_t* dest = data+(pitch*jj);
            
            for (i=sx,ii=dx;i<w;i++,ii++) {
                uint32_t* spix=(uint32_t*)source;
                uint32_t* dpix=(uint32_t*)dest;
                
                dpix[ii]=spix[i];
            }
        }
    }
    
    if (flags==1) {
        for (j=sy,jj=dy;j<h;j++,jj++) {
            uint8_t* source = src.data+(src.pitch*j);
            uint8_t* dest = data+(pitch*jj);
            
            for (i=sx,ii=dx;i<w;i++,ii++) {
                uint32_t* spix=(uint32_t*)source;
                uint32_t* dpix=(uint32_t*)dest;
                
                if ((spix[i] & 0x000000ff)>0) {
                    dpix[ii]=spix[i];
                }
            }
        }
    }
    
}

bool Surface::is_good()
{
    return (data!=nullptr);
}
