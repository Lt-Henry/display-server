

#include "drm.hpp"

#include <cstdint>
#include <iostream>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <libdrm/drm.h>
#include <libdrm/drm_mode.h>

using namespace std;
using namespace ds;

Drm::Drm(const char* path)
{

    // init strings
    
    connector_type[DRM_MODE_CONNECTOR_Unknown] = "unknown";
    connector_type[DRM_MODE_CONNECTOR_VGA] = "vga";
    connector_type[DRM_MODE_CONNECTOR_eDP] = "eDP";
    connector_type[DRM_MODE_CONNECTOR_HDMIA] = "hdmi";
    connector_type[DRM_MODE_CONNECTOR_HDMIB] = "hdmi";
    
    connector_status[0] = "Disconnected";
    connector_status[1] = "Connected";


    fd = open(path,O_RDWR | O_CLOEXEC);
    
    clog<<"card0:"<<endl;
    clog<<"fd:"<<fd<<endl;
    
    // become master
    ioctl(fd, DRM_IOCTL_SET_MASTER, 0);
    
    struct drm_mode_card_res res={0};
    
    ioctl(fd, DRM_IOCTL_MODE_GETRESOURCES, &res);
    
    clog<<"crtcs:"<<res.count_crtcs<<endl;
    clog<<"encoders:"<<res.count_encoders<<endl;
    clog<<"connectors:"<<res.count_connectors<<endl;
    clog<<"framebuffers:"<<res.count_fbs<<endl;
    
    
    uint64_t res_crtc_buf[10]={0};
    uint64_t res_conn_buf[10]={0};
    uint64_t res_fb_buf[10]={0};
    uint64_t res_enc_buf[10]={0};
    
    res.crtc_id_ptr=(uint64_t)res_crtc_buf;
    res.connector_id_ptr=(uint64_t)res_conn_buf;
    res.fb_id_ptr=(uint64_t)res_fb_buf;
    res.encoder_id_ptr=(uint64_t)res_enc_buf;
    
    ioctl(fd, DRM_IOCTL_MODE_GETRESOURCES, &res);
    
    clog<<"connectors:"<<endl;
    
    for (size_t n=0;n<res.count_connectors;n++) {
        struct drm_mode_get_connector conn={0};

        conn.connector_id=res_conn_buf[n];

        ioctl(fd, DRM_IOCTL_MODE_GETCONNECTOR, &conn);
        
        clog<<"    id: "<<conn.connector_id<<endl;
        clog<<"    type: "<<connector_type[conn.connector_type]<<endl;
        clog<<"    encoders: "<<conn.count_encoders<<endl;
        clog<<"    modes: "<<conn.count_modes<<endl;
        clog<<"    connection: "<<connector_status[conn.connection]<<endl;
        
        struct drm_mode_modeinfo conn_mode_buf[20]={0};
        uint64_t conn_prop_buf[20]={0},
            conn_propval_buf[20]={0},
            conn_enc_buf[20]={0};
            
        conn.modes_ptr=(uint64_t)conn_mode_buf;
        conn.props_ptr=(uint64_t)conn_prop_buf;
        conn.prop_values_ptr=(uint64_t)conn_propval_buf;
        conn.encoders_ptr=(uint64_t)conn_enc_buf;
        ioctl(fd, DRM_IOCTL_MODE_GETCONNECTOR, &conn);
        
        for (size_t m=0;m<conn.count_modes;m++) {
            clog<<"    -> "<<conn_mode_buf[m].name <<endl;
        }
    }
    
    clog<<"crtcs:"<<endl;
    
    for (size_t n=0;n<res.count_crtcs;n++) {
        struct drm_mode_crtc crtc={0};
        
        crtc.crtc_id=res_crtc_buf[n];
        ioctl(fd, DRM_IOCTL_MODE_GETCRTC, &crtc);
        
        clog<<"    id: "<<crtc.crtc_id<<endl;
        clog<<"    connectors: "<<crtc.count_connectors<<endl;
        
    }
    




    
    // drop master
    ioctl(fd, DRM_IOCTL_DROP_MASTER, 0);
}

Drm::~Drm()
{
    close(fd);
}
