#pragma once

#include "fwd.h"

#include "sgl/core/short_vector.h"
#include "sgl/math/matrix_types.h"

#include <string>

namespace sgl::render {

// struct PrimData {
//     PrimID parent;
//     short_vector<PrimID, 4> children;
//     short_vector<ComponentID, 4> components;
// };

struct StageObjectData {
    StageObjectKind kind;
    std::string path;

    // StageObjectData* next_create;
};

struct StageXformData : StageObjectData {
    XformID parent;
    float4x4 matrix;
};

struct StageLightData : StageObjectData {
    XformID parent;
};

} // namespace sgl::render
