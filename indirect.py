import sgl
from pathlib import Path

for mode in [3]: #, 2, 3, 4]:
    device = sgl.Device(
        compiler_options={
            "include_paths": [Path(__file__).parent],
            "defines": {
                "MODE": str(mode),
            },
            "dump_intermediates": True,
            "dump_intermediates_prefix": f"mode_{mode}_",
        }
    )

    vertices = device.create_buffer(size=4, struct_size=4, usage=sgl.ResourceUsage.shader_resource)
    result = device.create_buffer(size=4, struct_size=4, usage=sgl.ResourceUsage.unordered_access)

    program = device.load_program("test.slang", ["main"])
    kernel = device.create_compute_kernel(program=program)
    kernel.dispatch(thread_count=[1,1,1], vars={"vertices": vertices, "result": result})
    print(program)
