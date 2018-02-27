

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
    connection=conn.connection;
    encoder_id=conn.encoder_id;
    
    uint32_t* encoder_buf = nullptr;
    struct drm_mode_modeinfo* modeinfo_buf = nullptr;
    uint32_t* prop_buf = nullptr;
    uint32_t* prop_value_buf = nullptr;
    
    encoder_ids.clear();
    
    if (conn.count_encoders>0) {
        encoder_buf = new uint32_t[conn.count_encoders];
    }
    
    modes.clear();
    
    if (conn.count_modes>0) {
        modeinfo_buf = new struct drm_mode_modeinfo[conn.count_modes];
    }
    
    prop_ids.clear();
    prop_value_ids.clear();
    
    if (conn.count_props>0) {
        prop_buf = new uint32_t[conn.count_props];
        prop_value_buf = new uint32_t[conn.count_props];
    }
    
    conn.modes_ptr=(uint64_t)modeinfo_buf;
    conn.props_ptr=(uint64_t)prop_buf;
    conn.prop_values_ptr=(uint64_t)prop_value_buf;
    conn.encoders_ptr=(uint64_t)encoder_buf;
    
    ioctl(dri_fd, DRM_IOCTL_MODE_GETCONNECTOR, &conn)
    
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

vector<Encoder> Connector::get_encoders()
{
    vector<Encoder> encoders;
    
    struct drm_mode_get_connector conn={0};
    uint32_t* buffer;
    
    buffer = new uint32_t[this->encoders];
    
    conn.connector_id=this->id;
    ioctl(fd, DRM_IOCTL_MODE_GETCONNECTOR, &conn);
    
    conn.encoders_ptr=(uint64_t)buffer;
    ioctl(fd, DRM_IOCTL_MODE_GETCONNECTOR, &conn);
    
    for (int n=0;n<this->encoders;n++) {
        encoders.push_back(Encoder(fd,buffer[n]));
    }
    
    delete [] buffer;
    
    return encoders;
}
