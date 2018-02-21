

#include <iostream>

#include "gpu.hpp"

using namespace std;
using namespace ds::drm;


int main(int argc,char* argv[])
{

    Gpu card("/dev/dri/card0");
    
    vector<Connector> connectors = card.get_connectors();
    
    for (Connector& c:connectors) {
        clog<<"* "<<c.id<<endl;
        clog<<"* "<<c.type<<endl;
        clog<<"* "<<c.connection<<endl<<endl;
    }
    
    return 0;
}
