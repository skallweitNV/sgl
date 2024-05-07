#include "stage.h"
#include "pool.h"
#include "stage_data.h"

namespace sgl::render {

StageObject::StageObject(ref<Stage> stage, std::string_view path, StageObjectKind kind)
    : m_stage(std::move(stage))
    , m_kind(kind)
{
    SGL_ASSERT(m_stage);
    SGL_UNUSED(path);
}

StageObject::~StageObject() { }

// ----------------------------------------------------------------------------
// Xform
// ----------------------------------------------------------------------------

Xform::Xform(ref<Stage> stage, std::string_view path)
    : StageObject(std::move(stage), path, StageObjectKind::xform)
{
    m_id = m_stage->_create_object<StageObjectKind::xform>();
    StageXformData* data = m_stage->_get_object<StageObjectKind::xform, StageXformData>(m_id);
    data->kind = StageObjectKind::xform;
    data->path = path;
}

Xform::~Xform() { }

// ----------------------------------------------------------------------------
// Light
// ----------------------------------------------------------------------------

Light::Light(ref<Stage> stage, std::string_view path)
    : StageObject(std::move(stage), path, StageObjectKind::light)
{
    m_id = m_stage->_create_object<StageObjectKind::light>();
    StageLightData* data = m_stage->_get_object<StageObjectKind::light, StageLightData>(m_id);
    data->kind = StageObjectKind::light;
    data->path = path;
}

Light::~Light() { }

// ----------------------------------------------------------------------------
// Stage::Impl
// ----------------------------------------------------------------------------

struct Stage::Impl {
    Pool<StageObjectKind::xform, XformData> xforms;
    Pool<StageObjectKind::light, LightData> lights;

    // TODO add created object list (lock-free)
    // TODO add modified object list (lock-free)
};

Stage::Stage()
{
    m_impl = std::make_unique<Impl>();
}

Stage::~Stage() { }

StageObjectID Stage::_create_object(StageObjectKind kind)
{
    switch (kind) {
    case StageObjectKind::xform:
        return StageObjectID{m_impl->xforms.create()};
    case StageObjectKind::light:
        return StageObjectID{m_impl->lights.create()};
    default:
        SGL_UNREACHABLE();
    }
}

void Stage::_destroy_object(StageObjectID id)
{
    switch (id.kind()) {
    case StageObjectKind::xform:
        m_impl->xforms.destroy(id.as<StageObjectKind::xform>());
        return;
    case StageObjectKind::light:
        m_impl->lights.destroy(id.as<StageObjectKind::light>());
        return;
    default:
        SGL_UNREACHABLE();
    }
}

void* Stage::_get_object(StageObjectID id)
{
    switch (id.kind()) {
    case StageObjectKind::xform:
        return m_impl->xforms.get(id.as<StageObjectKind::xform>());
    case StageObjectKind::light:
        return m_impl->lights.get(id.as<StageObjectKind::light>());
    default:
        SGL_UNREACHABLE();
    }
}


void test()
{
    XformID xform_id;
    StageObjectID stage_object_id{xform_id};
    XformID xform_id2 = stage_object_id.as<StageObjectKind::xform>();

    ref<Stage> stage = ref(new Stage());
    ref<Xform> xform = ref(new Xform(stage, "/root"));
}


} // namespace sgl::render
