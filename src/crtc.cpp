

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


Crtc::Crtc()
{
    fd=0;
    id=0;
}

Crtc::Crtc(int fd, uint32_t id)
{
    this->fd=fd;
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
    
    clog<<"current crtc fb id: "<<crtc.fb_id;
    
    crtc.fb_id=fb.id;
    crtc.set_connectors_ptr=(uint64_t)&conn_id;
    crtc.count_connectors=1;
    crtc.mode=conn.modes[0];
    crtc.mode_valid=1;
    ioctl(fd, DRM_IOCTL_MODE_SETCRTC, &crtc);
}
