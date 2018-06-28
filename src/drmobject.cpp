
#include "drmobject.hpp"

using namespace ds;

drm::Object::Object()
{
    fd=-1;
}

drm::Object::Object(int fd)
{
    this->fd=fd;
}

drm::Object::~Object()
{

}

bool drm::Object::is_valid()
{
    return (fd>=0);
}

int drm::Object::get_fd()
{
    return fd;
}
