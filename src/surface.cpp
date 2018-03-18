
#include "surface.hpp"


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

Surface::Surface(int width,int height)
{
    this->width=width;
    this->height=height;
    
    bpp = 4;
    pitch = width*bpp;
    
    data = new uint8_t[width*height*bpp];
}

Surface::~Surface()
{
}

void Surface::destroy()
{
    delete [] data;
    
    data = nullptr;
}

void Surface::fill(uint32_t pixel)
{

}

void Surface::blit(Surface& surface,int x,int y)
{
}
