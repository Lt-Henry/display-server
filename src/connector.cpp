

#include <iostream>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <libdrm/drm.h>
#include <libdrm/drm_mode.h>

#include "connector.hpp"

using namespace std;
using namespace ds::drm;

static const char* names[] = {
    "Unknown",
    "VGA",
    "DVII",
    "DVID",
    "DVIA",
    "Composite",
    "SVIDEO",
    "LVDS",
    "Component",
    "9PinDIN",
    "DisplayPort",
    "HDMIA",
    "HDMIB",
    "TV",
    "eDP",
    "VIRTUAL",
    "DSI",
    "DPI"
};

Connector::Connector()
{
    this->fd=0;
    this->id=0;
    
    connection=3;
    type=0;
}

Connector::Connector(int fd,uint32_t id)
{
    this->fd=fd;
    this->id=id;
    
    update();
}

Connector::~Connector()
{
}

void Connector::update()
{
    struct drm_mode_get_connector conn={0};

    conn.connector_id=this->id;
    ioctl(fd, DRM_IOCTL_MODE_GETCONNECTOR, &conn);

    
    type=conn.connector_type;
    encoders=conn.count_encoders;
    modes=conn.count_modes;
    connection=conn.connection;
}

bool Connector::is_connected()
{
    return (connection==1);
}

string Connector::get_type_name()
{
    return string(names[type]);
}

void Connector::get_modes()
{
    struct drm_mode_get_connector conn={0};
    struct drm_mode_modeinfo* modeinfos;
    modeinfos = new struct drm_mode_modeinfo[this->modes];
    
    conn.connector_id=this->id;
    ioctl(fd, DRM_IOCTL_MODE_GETCONNECTOR, &conn);
    
    conn.modes_ptr=(uint64_t)modeinfos;
    ioctl(fd, DRM_IOCTL_MODE_GETCONNECTOR, &conn);
    
    for (int n=0;n<this->modes;n++) {
        clog<<"m "<<modeinfos[n].name<<endl;
    }
    
    delete [] modeinfos;
}
