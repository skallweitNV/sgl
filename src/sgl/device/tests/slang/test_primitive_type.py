# SPDX-License-Identifier: Apache-2.0

import sys
import pytest
import sgl
import numpy as np
from pathlib import Path

sys.path.append(str(Path(__file__).parent.parent))
import helpers

ELEMENT_COUNT = 1024

TYPES = ["uint", "float"]


@pytest.mark.parametrize("device_type", helpers.DEFAULT_DEVICE_TYPES)
@pytest.mark.parametrize("type", TYPES)
def test_primitive_type(device_type, type):
    pytest.skip("Not implemented")
    device = helpers.get_device(type=device_type)

    session = device.create_slang_session(
        compiler_options={"include_paths": [Path(__file__).parent]}
    )

    # constants = "\n".join(
    #     [
    #         "import test_primitive_types;",
    #         f"export struct TEST : ITest = Test_{type};",
    #     ]
    # )

    # program = session.load_program(
    #     module_name="test_primitive_type.slang",
    #     entry_point_names=["main"],
    #     additional_source=constants,
    # )

    program = session.load_program(module_name="test_primitive_type.slang", entry_point_names=[f"main_{type}"])

    kernel = device.create_compute_kernel(program)

    # result = device.create_structured_buffer(
    #     element_count=1,
    #     struct_type=program.reflection.result,
    #     usage=sgl.ResourceUsage.unordered_access,
    # )

    # kernel.dispatch(thread_count=[1, 1, 1])

    # expected = {"AddOp": 3, "SubOp": -1, "MulOp": 2}[op]

    # assert result.to_numpy().view(dtype=np.float32)[0] == expected

    # # np.random.seed(123)
    # # data = np.random.rand(ELEMENT_COUNT, 2).astype(np.float16)

    # ctx = helpers.dispatch_compute(
    #     device=device,
    #     path=Path(__file__).parent / "test_primitive_types.slang",
    #     entry_point="main",
    #     thread_count=[ELEMENT_COUNT, 1, 1],
    #     # buffers={
    #     #     "data": {"data": data},
    #     #     "result": {"element_count": ELEMENT_COUNT},
    #     # },
    # )

    # # expected = data.view(np.uint32).flatten()
    # # result = ctx.buffers["result"].to_numpy().view(np.uint32).flatten()
    # # assert np.all(result == expected)


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
