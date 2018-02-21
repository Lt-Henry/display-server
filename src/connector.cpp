

#include <iostream>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <libdrm/drm.h>
#include <libdrm/drm_mode.h>

#include "connector.hpp"

using namespace std;
using namespace ds::drm;

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
    clog<<"id:"<<id<<endl;
    
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

    clog<<"-"<<conn.connector_type<<endl;
    
    type=conn.connector_type;
    encoders=conn.count_encoders;
    modes=conn.count_modes;
    connection=conn.connection;
}
