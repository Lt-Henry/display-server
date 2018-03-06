
#include "dumb.hpp"
#include "gpu.hpp"

#include <iostream>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <libdrm/drm.h>
#include <libdrm/drm_mode.h>

using namespace std;
using namespace ds::drm;

DumbBuffer::DumbBuffer()
{
    fd=0;
    handle=0;
}

DumbBuffer DumbBuffer::create(Gpu& gpu,int width,int height)
{
    this->fd=gpu.fd;
}
