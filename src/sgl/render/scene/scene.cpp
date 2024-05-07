#include "scene.h"

#include "object_id.h"

namespace sgl::render {

Scene::Scene(ref<Device> device)
    : m_device(std::move(device))
{
    SGL_CHECK_NOT_NULL(m_device);
}

Scene::~Scene()
{
}

} // namespace sgl::render
