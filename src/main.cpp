

#include <iostream>

#include "gpu.hpp"

using namespace std;
using namespace ds::drm;


int main(int argc,char* argv[])
{

    Gpu card("/dev/dri/card0");
    
    vector<Connector> connectors = card.get_connectors();
    
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
    
        main_output->get_modes();
        
        vector<Encoder> encoders;
        
        card.set_master();
        encoders = main_output->get_encoders();
        card.drop_master();
        
        for (Encoder & e: encoders) {
            clog<<"[e] "<<e.get_type_name()<<endl;
            clog<<e.crtc_id<<endl;
        }
    }
    
    return 0;
}
