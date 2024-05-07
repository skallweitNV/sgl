// SPDX-License-Identifier: Apache-2.0

#include "nanobind.h"

#include "sgl/render/scene/stage.h"

namespace sgl::render {

void register_stage(nanobind::module_ m)
{
    nb::class_<Stage, Object>(m, "Stage", D_NA(Stage)).def(nb::init<>());

    nb::class_<StageObject, Object>(m, "StageObject", D_NA(StageObject))
        .def("kind", &StageObject::kind, D_NA(StageObject, kind));

    nb::class_<Xform, StageObject>(m, "Xform", D_NA(Xform))
        .def(nb::init<ref<Stage>, std::string_view>(), "stage"_a, "path"_a, D_NA(Xform));

    nb::class_<Light, StageObject>(m, "Light", D_NA(Light))
        .def(nb::init<ref<Stage>, std::string_view>(), "stage"_a, "path"_a, D_NA(Light));
}

} // namespace sgl::render

SGL_PY_EXPORT(render_stage)
{
    using namespace sgl;
    using namespace sgl::render;

    nb::module_ render = m.attr("render");
    register_stage(render);
}
