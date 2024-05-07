// SPDX-License-Identifier: Apache-2.0

#include "sgl/sgl.h"
#include "sgl/render/scene/stage.h"

using namespace sgl;
using namespace sgl::render;

int main()
{
    sgl::static_init();

    {
        ref<Stage> stage = ref(new Stage());
        ref<Xform> xform1 = ref(new Xform(stage, "/root"));
        ref<Xform> xform2 = ref(new Xform(stage, "/root/child"));
    }

    sgl::static_shutdown();
    return 0;
}
