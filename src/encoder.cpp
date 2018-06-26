#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <libdrm/drm.h>
#include <libdrm/drm_mode.h>

#include "encoder.hpp"

using namespace std;
using namespace ds::drm;

static const char* names[] =
{
    "NONE",
    "DAC",
    "TMDS",
    "LVDS",
    "TVDAC",
    "VIRTUAL",
    "DSI",
    "DPMST",
    "DPI"
};

Encoder::Encoder() : Object()
{
    id=0;
    type=0;
}

Encoder::Encoder(int fd,uint32_t id) : Object(fd)
{
    this->id=id;
    update();
}

void Encoder::update()
{
    struct drm_mode_get_encoder enc={0};
    
    enc.encoder_id=id;
    ioctl(fd, DRM_IOCTL_MODE_GETENCODER, &enc);
    
    type=enc.encoder_type;
    crtc_id=enc.crtc_id;
    
}

string Encoder::get_type_name()
{
    return string(names[type]);
}

Crtc Encoder::get_crtc()
{
    return Crtc(fd,crtc_id);
}
