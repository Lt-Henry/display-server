

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
    struct drm_mode_card_res res={0};
    
    set_master();
    
    ioctl(fd, DRM_IOCTL_MODE_GETRESOURCES, &res);
    
    if (res.count_connectors>0) {
    
        uint32_t res_fb_buf[10]={0},
        res_crtc_buf[10]={0},
        res_enc_buf[10]={0};

        res.fb_id_ptr=(uint64_t)res_fb_buf;
        res.crtc_id_ptr=(uint64_t)res_crtc_buf;
        res.encoder_id_ptr=(uint64_t)res_enc_buf;
    
        uint32_t* ids = new uint32_t[res.count_connectors];
        res.connector_id_ptr=(uint64_t)ids;
        
        ioctl(fd, DRM_IOCTL_MODE_GETRESOURCES, &res);
        
        for (size_t n=0;n<res.count_connectors;n++) {
            connectors.push_back(Connector(fd,ids[n]));
        }
        
        delete [] ids;
    }
    
    drop_master();
    
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
    
    uint32_t* fb_buf;
    uint32_t* crtc_buf;
    uint32_t* connector_buf;
    
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
    
    delete [] fb_buf;
    
}

