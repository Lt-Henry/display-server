

#include "crtc.hpp"
#include "connector.hpp"

#include <iostream>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <libdrm/drm.h>
#include <libdrm/drm_mode.h>

using namespace std;
using namespace ds::drm;


Crtc::Crtc() : Object()
{
    id=0;
}

Crtc::Crtc(int fd, uint32_t id) : Object(fd)
{
    this->id=id;
    
    update();
}

void Crtc::update()
{

}

void Crtc::add_fb(Connector& conn,DumbBuffer& fb)
{
    struct drm_mode_crtc crtc={0};
    uint64_t conn_id = conn.id;

    crtc.crtc_id=id;
    ioctl(fd, DRM_IOCTL_MODE_GETCRTC, &crtc);
    
    clog<<"current crtc fb id: "<<crtc.fb_id<<endl;
    
    crtc.fb_id=fb.id;
    crtc.set_connectors_ptr=(uint64_t)&conn_id;
    crtc.count_connectors=1;
    crtc.mode=conn.modes[0];
    crtc.mode_valid=1;
    ioctl(fd, DRM_IOCTL_MODE_SETCRTC, &crtc);
}

void Crtc::page_flip(DumbBuffer& fb)
{
    struct drm_mode_crtc_page_flip flip = {0};
    
    flip.crtc_id=id;
    flip.fb_id=fb.id;
    flip.flags=DRM_MODE_PAGE_FLIP_ASYNC;
    
    ioctl(fd, DRM_IOCTL_MODE_PAGE_FLIP, &flip);
}

void Crtc::vsync()
{
    // HACK HACK HACK
    char buffer[128];
    int len;
    
    struct drm_event* event;
    
    len=read(fd,buffer,sizeof (struct drm_event));
    clog<<"event size:"<<len<<endl;
    if (len>0) {
        event=(struct drm_event*)buffer;
        clog<<"event: "<<event->type<<endl;
    }
}
