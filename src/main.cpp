

#include <iostream>
#include <chrono>
#include <unistd.h>


#include <sys/ioctl.h>
#include <sys/kd.h>
#include <fcntl.h>

#include "gpu.hpp"
#include "dumb.hpp"
#include "surface.hpp"

using namespace std;
using namespace ds;
using namespace ds::drm;


int main(int argc,char* argv[])
{

    int tty_fd = open("/dev/tty0", O_RDWR);
    ioctl(tty_fd,KDSETMODE,KD_GRAPHICS);

    Gpu card("/dev/dri/card0");
    
    card.set_master();
    
    if (card.support_dumb_buffer()) {
        clog<<"Dumb buffer supported"<<endl;
    }
    
    card.update();
    
    
    vector<Connector> connectors = card.get_connectors();
    
    //card.drop_master();
    
    Connector* main_output=nullptr;
    
    for (Connector& c:connectors) {
        clog<<"* "<<c.id<<endl;
        clog<<"* "<<c.get_type_name()<<endl;
        clog<<"* "<<c.is_connected()<<endl<<endl;
        
        if (c.is_connected() and main_output==nullptr) {
            main_output=&c;
        }
    }
    
    if (main_output) {
        clog<<"Found one valid output"<<endl;
    
        vector<struct drm_mode_modeinfo> modes = main_output->get_modes();
        
        Encoder encoder;
        Crtc crtc;
        
        encoder = main_output->get_encoder();
        crtc = encoder.get_crtc();

        clog<<"Using crtc "<<crtc.id<<endl;
        clog<<"with: "<<modes[0].hdisplay<<"x"<<modes[0].vdisplay<<endl;
        
        DumbBuffer b1(card,modes[0].hdisplay,modes[0].vdisplay);
        DumbBuffer b2(card,modes[0].hdisplay,modes[0].vdisplay);
        Surface box(64,64);
        box.fill(0xf7e26bff);
        
        DumbBuffer* dumb=&b1;
        
        crtc.add_fb(*main_output,b1);
        //card.drop_master();
        
        int x=8;
        int y=0;
        
        
        int dx=1;
        int dy=1;
        
        int count=0;
        
        int frames=0;
        
        auto start = chrono::steady_clock::now();
        
        while (count<10) {
        
            dumb->fill(0xffffffff);
        
            if (x==0) {
                dx=1;
                count++;
                clog<<"x"<<endl;
            }
            
            if (x==dumb->width) {
                dx=-1;
                count++;
                clog<<"w"<<endl;
            }
            
            if (y==0) {
                dy=1;
                count++;
                clog<<"y"<<endl;
            }
            
            if (y==dumb->height) {
                dy=-1;
                count++;
                clog<<"h"<<endl;
            }
            
            x=x+dx;
            y=y+dy;
            
            dumb->blit(box,x,y);
          
            dumb->dirty();
            crtc.page_flip(*dumb);
            
            
            if (dumb==&b1) {
                dumb=&b2;
            }
            else {
                dumb=&b1;
            }
            
            frames++;
            
            auto end = chrono::steady_clock::now();
            double fpstime = chrono::duration <double, milli> (end-start).count();
            
            if (fpstime>1000) {
                clog<<"* fps: "<<frames<<endl;
                frames=0;
                start=end;
            }
            usleep(15000);
        }
        
        b1.destroy();
        b2.destroy();
    }
    
    card.drop_master();
    
    ioctl(tty_fd,KDSETMODE,KD_TEXT);
    
    clog<<"bye"<<endl;
    
    return 0;
}
