#pragma once

#include "fwd.h"

#include <cstdint>

namespace sgl::render {

enum class StageObjectKind {
    unknown,
    xform,
    light,
};

template<StageObjectKind Kind>
class ObjectID {
public:
    ObjectID() = default;
    explicit ObjectID(uint64_t value)
        : m_value(value)
    {
    }

    uint64_t index() const { return m_value; }

private:
    static constexpr uint64_t INVALID = uint64_t(-1);

    uint64_t m_value{INVALID};

    friend class StageObjectID;
};

class StageObjectID {
public:
    StageObjectID() = default;

    template<StageObjectKind Kind>
    explicit StageObjectID(ObjectID<Kind> id)
        : m_kind(Kind)
        , m_value(id.m_value)
    {
    }

    StageObjectKind kind() const { return m_kind; }

    template<StageObjectKind Kind>
    ObjectID<Kind> as() const
    {
        return ObjectID<Kind>{m_value};
    }

private:
    static constexpr uint64_t INVALID = uint64_t(-1);

    StageObjectKind m_kind{StageObjectKind::unknown};
    uint64_t m_value{INVALID};
};

using XformID = ObjectID<StageObjectKind::xform>;
using LightID = ObjectID<StageObjectKind::light>;

} // namespace sgl::render
