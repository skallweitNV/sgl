// SPDX-License-Identifier: Apache-2.0

#include "nanobind.h"

#include "sgl/device/shader_object.h"

SGL_PY_EXPORT(device_shader_object)
{
    using namespace sgl;

    nb::class_<ShaderObject, Object>(m, "ShaderObject", D(ShaderObject));
    nb::class_<TransientShaderObject, ShaderObject>(m, "TransientShaderObject", D(TransientShaderObject));
    nb::class_<MutableShaderObject, ShaderObject>(m, "MutableShaderObject", D(MutableShaderObject));
}
