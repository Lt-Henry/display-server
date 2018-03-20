
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

DumbBuffer::DumbBuffer(Gpu& gpu,int width,int height)
{
    
    fd=gpu.fd;
    
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
    
    ioctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &create_dumb);
    
    handle=create_dumb.handle;
    
    cmd_dumb.width=create_dumb.width;
    cmd_dumb.height=create_dumb.height;
    cmd_dumb.bpp=create_dumb.bpp;
    cmd_dumb.pitch=create_dumb.pitch;
    cmd_dumb.depth=24;
    cmd_dumb.handle=create_dumb.handle;
    
    ioctl(fd,DRM_IOCTL_MODE_ADDFB,&cmd_dumb);
    
    map_dumb.handle=create_dumb.handle;
    id=cmd_dumb.fb_id;
    
    ioctl(fd,DRM_IOCTL_MODE_MAP_DUMB,&map_dumb);
    
    this->width=create_dumb.width;
    this->height=create_dumb.height;
    pitch=create_dumb.pitch;
    size=create_dumb.size;
    
    data = (uint8_t*)mmap(0, create_dumb.size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, map_dumb.offset);
    
    clog<<"dumb:"<<width<<"x"<<height<<endl;
    clog<<"mapped as "<<(uint64_t)data<<endl;
    clog<<"size: "<<create_dumb.size<<endl;
    clog<<"pitch: "<<create_dumb.pitch<<endl;
    
}

void DumbBuffer::destroy()
{
    munmap(data,size);
    
    struct drm_mode_fb_cmd cmd_fb={0};
    
    cmd_fb.fb_id=id;
    ioctl(fd,DRM_IOCTL_MODE_RMFB,&cmd_fb);
    
    struct drm_mode_destroy_dumb destroy_dumb={0};
    
    destroy_dumb.handle=handle;
    ioctl(fd,DRM_IOCTL_MODE_DESTROY_DUMB,&destroy_dumb);
    
}

void DumbBuffer::dirty()
{
    struct drm_mode_fb_dirty_cmd dirty = {0};
    struct drm_clip_rect clip_rect = {
        0, 0, width, height
    };

    dirty.fb_id = id;
    dirty.clips_ptr = (uint64_t)(&clip_rect);
    dirty.num_clips = 1;

    ioctl(fd, DRM_IOCTL_MODE_DIRTYFB, &dirty);
}
