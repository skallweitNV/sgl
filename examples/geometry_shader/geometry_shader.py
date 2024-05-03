# SPDX-License-Identifier: Apache-2.0

import sgl
import numpy as np
from pathlib import Path

EXAMPLE_DIR = Path(__file__).parent

device = sgl.Device(
    type=sgl.DeviceType.vulkan,
    enable_print=True,
    enable_debug_layers=True,
    compiler_options={
        "include_paths": [EXAMPLE_DIR],
        "dump_intermediates": True,
        "shader_model": sgl.ShaderModel.sm_6_5,
    },
)

framebuffer = device.create_framebuffer([])

sum_buffer = device.create_buffer(size=16 * 4, usage=sgl.ResourceUsage.unordered_access)

program = device.load_program(
    "geometry_shader.slang", ["vertex_main", "geometry_main", "fragment_main"]
)
pipeline = device.create_graphics_pipeline(
    program=program,
    input_layout=None,
    framebuffer_layout=framebuffer.layout,
    primitive_type=sgl.PrimitiveType.point,
    rasterizer={
        "enable_conservative_rasterization": False,
    }
)

command_buffer = device.create_command_buffer()
command_buffer.clear_resource_view(sum_buffer.get_uav(), sgl.uint4(0))
with command_buffer.encode_render_commands(framebuffer) as encoder:
    shader_object = encoder.bind_pipeline(pipeline)
    sgl.ShaderCursor(shader_object).sum_buffer = sum_buffer
    encoder.set_primitive_topology(sgl.PrimitiveTopology.point_list)
    encoder.set_viewport_and_scissor_rect(
        {
            "width": 1024,
            "height": 1024,
        }
    )
    encoder.draw(16)
command_buffer.submit()

device.flush_print()

print(sum_buffer.to_numpy().view(np.uint32))

# sgl.tev.show(render_texture, "geometry_shader")
