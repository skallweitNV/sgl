#pragma once

#include "sgl/core/object.h"
#include "sgl/device/device.h"

namespace sgl::render {

class SGL_API Scene : public Object {
    SGL_OBJECT(Scene)
public:
    Scene(ref<Device> device);
    virtual ~Scene();

private:
    ref<Device> m_device;
};

} // namespace sgl::render
