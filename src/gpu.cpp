

#include "gpu.hpp"

#include <cstdint>
#include <iostream>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <libdrm/drm.h>
#include <libdrm/drm_mode.h>

using namespace std;
using namespace ds::drm;


Gpu::Gpu(string path)
{

    fd = open(path.c_str(),O_RDWR | O_CLOEXEC);
    
    clog<<"card0:"<<endl;
    clog<<"fd:"<<fd<<endl;
    
}

Gpu::~Gpu()
{
    close(fd);
}

vector<Connector> Gpu::get_connectors()
{
    vector<Connector> connectors;
    
    for (uint32_t id : connector_ids) {
        connectors.push_back(fd,id);
    }
    
    return connectors;
}

void Gpu::set_master()
{
    ioctl(fd, DRM_IOCTL_SET_MASTER, 0);
}

void Gpu::drop_master()
{
    ioctl(fd, DRM_IOCTL_DROP_MASTER, 0);
}

void Gpu::update()
{
    struct drm_mode_card_res res={0};
    
    uint32_t* fb_buf = nullptr;
    uint32_t* crtc_buf = nullptr;
    uint32_t* connector_buf = nullptr;
    uint32_t* encoder_buf = nullptr;
    
    ioctl(fd, DRM_IOCTL_MODE_GETRESOURCES, &res);
    
    
    // frame buffers
    fb_ids.clear();
    
    if (res.count_fbs>0) {
        fb_buf = new uint32_t[res.count_fbs];
    }
    
    // crtcs
    crtc_ids.clear();
    
    if (res.count_crtcs>0) {
        crtc_buf = new uint32_t[res.count_crtcs];
    }
    
    // connectors
    connector_ids.clear();
    
    if (res.count_connectors>0) {
        connector_buf = new uint32_t[res.count_connectors];
    }
    
    // encoders
    encoders_ids.clear();
    
    if (res.count_encoders>0) {
        encoder_buf = new uint32_t[res.count_encoders];
    }
    
    res.fb_id_ptr=(uint64_t)fb_buf;
    res.crtc_id_ptr=(uint64_t)crtc_buf;
    res.connector_id_ptr=(uint64_t)connector_buf;
    res.encoder_id_ptr=(uint64_t)encoder_buf;
    

    if (res.count_fbs>0) {
        for (size_t n=0;n<res.count_fbs;n++) {
            fb_ids.push_back(fb_buf[n]);
        }
        
        delete [] fb_buf;
    }
    
    if (res.count_crtcs>0) {
        for (size_t n=0;n<res.count_crtcs;n++) {
            crtc_ids.push_back(crtc_buf[n]);
        }
        
        delete [] crtc_buf;
    }
    
    if (res.count_connectors>0) {
        for (size_t n=0;n<res.count_connectors;n++) {
            connector_ids.push_back(connector_buf[n]);
        }
        
        delete [] connector_buf;
    }
    
    if (res.count_encoders>0) {
        for (size_t n=0;n<res.count_encoders;n++) {
            encoder_ids.push_back(encoder_buf[n]);
        }
        
        delete [] encoder_buf;
    }
}

