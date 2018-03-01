

#include <iostream>

#include "gpu.hpp"

using namespace std;
using namespace ds::drm;


int main(int argc,char* argv[])
{

    Gpu card("/dev/dri/card0");
    
    card.set_master();
    
    if (card.support_dumb_buffer()) {
        clog<<"Dumb buffer supported"<<endl;
    }
    
    card.update();
    
    
    vector<Connector> connectors = card.get_connectors();
    
    card.drop_master();
    
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

    }
    
    return 0;
}
