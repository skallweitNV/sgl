#pragma once

#include "fwd.h"
#include "stage_object_id.h"

#include "sgl/core/object.h"
#include "sgl/core/enum.h"

#include <string_view>
#include <memory>

namespace sgl::render {

class SGL_API StageObject : public Object {
    SGL_OBJECT(StageObject)
public:
    StageObject(ref<Stage> stage, std::string_view path, StageObjectKind kind);
    virtual ~StageObject();

    StageObjectKind kind() const { return m_kind; }

protected:
    ref<Stage> m_stage;
    StageObjectKind m_kind;
};

class SGL_API Xform : public StageObject {
    SGL_OBJECT(Xform)
public:
    Xform(ref<Stage> stage, std::string_view path);
    virtual ~Xform();

private:
    XformID m_id;
};

class SGL_API Light : public StageObject {
    SGL_OBJECT(Light)
public:
    Light(ref<Stage> stage, std::string_view path);
    virtual ~Light();

private:
    LightID m_id;
};

class SGL_API Stage : public Object {
    SGL_OBJECT(Stage)
public:
    Stage();
    ~Stage();

    StageObjectID _create_object(StageObjectKind kind);
    void _destroy_object(StageObjectID id);
    void* _get_object(StageObjectID id);

    template<StageObjectKind Kind>
    ObjectID<Kind> _create_object()
    {
        return _create_object(Kind).as<Kind>();
    }

    template<StageObjectKind Kind>
    void _destroy_object(ObjectID<Kind> id)
    {
        _destroy_object(StageObjectID{id});
    }

    template<StageObjectKind Kind, typename T>
    T* _get_object(ObjectID<Kind> id)
    {
        return static_cast<T*>(_get_object(StageObjectID{id}));
    }

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace sgl::render
