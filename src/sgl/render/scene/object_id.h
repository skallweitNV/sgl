#pragma once

#include "fwd.h"

#include <cstdint>

namespace sgl::render {

enum class ObjectKind {
    unknown,
    prim,
    xform,
    light,
};

struct ObjectIDLayout {
    static constexpr uint64_t INDEX_BITS = 56;
    static constexpr uint64_t KIND_BITS = 8;
    static_assert(INDEX_BITS + KIND_BITS == 64, "Invalid bit count");
    static constexpr uint64_t INDEX_MASK = (uint64_t(1) << INDEX_BITS) - 1;
    static constexpr uint64_t KIND_MASK = (uint64_t(1) << KIND_BITS) - 1;

    static constexpr uint64_t INVALID = uint64_t(-1);

    static inline bool is_valid(uint64_t value) { return value != INVALID; }

    static inline uint64_t encode(ObjectKind kind, uint64_t index)
    {
        SGL_ASSERT(index <= INDEX_MASK);
        SGL_ASSERT(uint64_t(kind) <= KIND_MASK);
        uint64_t value = index;
        value |= (uint64_t(kind) << INDEX_BITS);
    }

    static inline ObjectKind decode_kind(uint64_t value) { return ObjectKind(value >> INDEX_BITS); }
    static inline uint64_t decode_index(uint64_t value) { return value & INDEX_MASK; }
};

template<ObjectKind Kind>
class ObjectID {
public:
    ObjectID() = default;
    explicit ObjectID(uint64_t index)
        : m_value(ObjectIDLayout::encode(Kind, index))
    {
    }

    bool is_valid() const { return ObjectIDLayout::is_valid(m_value); }

    ObjectKind kind() const { return Kind; }
    uint64_t index() const { return ObjectIDLayout::decode_index(m_value); }

private:
    uint64_t m_value{ObjectIDLayout::INVALID};
};

class GenericObjectID {
public:
    GenericObjectID() = default;

    template<ObjectKind Kind>
    explicit GenericObjectID(ObjectID<Kind> id)
        : m_value(ObjectID::encode(Kind, id.index()))
    {
    }

    ObjectKind kind() const { return ObjectIDLayout::decode_kind(m_value); }
    uint64_t index() const { return ObjectIDLayout::decode_index(m_value); }

    template<ObjectKind Kind>
    ObjectID<Kind> as() const
    {
        return ObjectID<Kind>{index()};
    }

private:
    uint64_t m_value{ObjectIDLayout::INVALID};
};

using XformID = ObjectID<ObjectKind::xform>;
using LightID = ObjectID<ObjectKind::light>;

} // namespace sgl::render
