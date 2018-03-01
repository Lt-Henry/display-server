

#include "crtc.hpp"

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
