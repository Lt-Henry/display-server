
#include "dumb.hpp"
#include "gpu.hpp"

#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <libdrm/drm.h>
#include <libdrm/drm_mode.h>

using namespace std;
using namespace ds::drm;

DumbBuffer::DumbBuffer()
{
    fd=0;
    handle=0;
}

DumbBuffer DumbBuffer::create(Gpu& gpu,int width,int height)
{
    DumbBuffer dumb;
    
    dumb.fd=gpu.fd;
    
    struct drm_mode_create_dumb create_dumb={0};
    struct drm_mode_map_dumb map_dumb={0};
    struct drm_mode_fb_cmd cmd_dumb={0};
    
    create_dumb.width = width;
    create_dumb.height = height;
    create_dumb.bpp = 32;
    create_dumb.flags = 0;
    create_dumb.pitch = 0;
    create_dumb.size = 0;
    create_dumb.handle = 0;
    
    ioctl(dumb.fd, DRM_IOCTL_MODE_CREATE_DUMB, &create_dumb);
    
    dumb.handle=create_dumb.handle;
    
    cmd_dumb.width=create_dumb.width;
    cmd_dumb.height=create_dumb.height;
    cmd_dumb.bpp=create_dumb.bpp;
    cmd_dumb.pitch=create_dumb.pitch;
    cmd_dumb.depth=24;
    cmd_dumb.handle=create_dumb.handle;
    
    ioctl(dumb.fd,DRM_IOCTL_MODE_ADDFB,&cmd_dumb);
    
    map_dumb.handle=create_dumb.handle;
    dumb.id=cmd_dumb.fb_id;
    
    ioctl(dumb.fd,DRM_IOCTL_MODE_MAP_DUMB,&map_dumb);
    
    dumb.width=create_dumb.width;
    dumb.height=create_dumb.height;
    
    dumb.data = mmap(0, create_dumb.size, PROT_READ | PROT_WRITE, MAP_SHARED, dumb.fd, map_dumb.offset);
    
    return dumb;
}

void DumbBuffer::destroy()
{

}
