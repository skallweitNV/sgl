# SPDX-License-Identifier: Apache-2.0

import sys
import pytest
import sgl
import numpy as np
from pathlib import Path

sys.path.append(str(Path(__file__).parent.parent))
import helpers

ELEMENT_COUNT = 128

LANE_COUNT_CACHE: dict[sgl.DeviceType, int] = {}


def query_lane_count(device_type: sgl.DeviceType) -> int:
    if device_type in LANE_COUNT_CACHE:
        return LANE_COUNT_CACHE[device_type]

    device = helpers.get_device(device_type)
    ctx = helpers.dispatch_compute(
        device=device,
        path=Path(__file__).parent / "test_wave_ops.slang",
        entry_point="query_lane_count",
        thread_count=[1, 1, 1],
        buffers={"result": {"element_count": 1}},
    )
    lane_count = ctx.buffers["result"].to_numpy().view(np.uint32)[0]
    LANE_COUNT_CACHE[device_type] = lane_count
    return lane_count


@pytest.mark.parametrize("device_type", helpers.DEFAULT_DEVICE_TYPES)
def test_wave_lane_count(device_type):
    lane_count = query_lane_count(device_type)
    assert lane_count >= 4 and lane_count <= 128


@pytest.mark.parametrize("device_type", helpers.DEFAULT_DEVICE_TYPES)
def test_wave_match(device_type):
    lane_count = query_lane_count(device_type)
    if lane_count < 4 or lane_count > 128:
        pytest.skip("Unsupported lane count")

    device = helpers.get_device(device_type)

    # pick a set of values from a small set of unique values
    np.random.seed(123)
    UNIQUE_COUNT = 20
    unique = np.random.randint(
        low=0, high=0xFFFFFFFF, size=UNIQUE_COUNT, dtype=np.uint32
    )
    data = unique[np.random.randint(low=0, high=UNIQUE_COUNT - 1, size=ELEMENT_COUNT)]

    # CPU implementation of the kernel using WaveMatch
    # TODO naive implementation, could be faster
    def cpu_kernel(input: np.ndarray, lane_count: int) -> np.ndarray:
        result = np.zeros(ELEMENT_COUNT * 4, dtype=np.uint32)
        for i in range(ELEMENT_COUNT):
            first_lane = (i // lane_count) * lane_count
            current_lane_value = input[i]
            for j in range(lane_count):
                if input[first_lane + j] == current_lane_value:
                    result[i * 4 + j // 32] |= 1 << (j % 32)
        return result

    expected = cpu_kernel(data, lane_count)

    ctx = helpers.dispatch_compute(
        device=device,
        path=Path(__file__).parent / "test_wave_ops.slang",
        entry_point="test_wave_match",
        thread_count=[ELEMENT_COUNT, 1, 1],
        buffers={
            "data": {"data": data},
            "result": {"element_count": ELEMENT_COUNT},
        },
    )

    result = ctx.buffers["result"].to_numpy().view(np.uint32).flatten()

    # mask out undefined mask bits
    if lane_count <= 64:
        result[2::4] = 0
        result[3::4] = 0
    if lane_count <= 32:
        result[1::4] = 0
    if lane_count <= 16:
        result[0::4] &= 0xFFFF
    if lane_count <= 8:
        result[0::4] &= 0xFF
    if lane_count <= 4:
        result[0::4] &= 0xF

    assert np.all(result == expected)


@pytest.mark.parametrize("device_type", helpers.DEFAULT_DEVICE_TYPES)
@pytest.mark.parametrize("type", ["int", "float"])
def test_wave_max(device_type, type):
    lane_count = query_lane_count(device_type)
    if lane_count < 4 or lane_count > 128:
        pytest.skip("Unsupported lane count")

    device = helpers.get_device(device_type)

    dtype = np.int32 if type == "int" else np.float32

    np.random.seed(123)
    data = (np.random.rand(ELEMENT_COUNT) * 2000000000 - 1000000000).astype(dtype)

    # CPU implementation of the kernel using WaveMax
    def cpu_kernel(input: np.ndarray, lane_count: int) -> np.ndarray:
        min = np.min(input)
        max = np.reshape(
            np.where(input < 0, input, min),
            (ELEMENT_COUNT // lane_count, lane_count),
        ).max(axis=1)
        max = np.repeat(max, lane_count)
        return np.where(input < 0, max, input)

    expected = cpu_kernel(data, lane_count)

    ctx = helpers.dispatch_compute(
        device=device,
        path=Path(__file__).parent / "test_wave_ops.slang",
        entry_point=f"test_wave_max_{type}",
        thread_count=[ELEMENT_COUNT, 1, 1],
        buffers={
            "data": {"data": data},
            "result": {"element_count": ELEMENT_COUNT},
        },
    )

    result = ctx.buffers["result"].to_numpy().view(dtype).flatten()
    assert np.all(result == expected)


@pytest.mark.parametrize("device_type", helpers.DEFAULT_DEVICE_TYPES)
@pytest.mark.parametrize("conditional", [0])
def test_wave_min_max(device_type, conditional):
    lane_count = query_lane_count(device_type)
    if lane_count < 4 or lane_count > 128:
        pytest.skip("Unsupported lane count")

    device = helpers.get_device(device_type)

    np.random.seed(123)
    data = np.random.rand(ELEMENT_COUNT).astype(np.float32)

    # CPU implementation of the kernel using WaveMin and WaveMax
    def wave_min_max(
        input: np.ndarray, conditional: bool, lane_count: int
    ) -> np.ndarray:
        expected_min = np.reshape(input, (ELEMENT_COUNT // lane_count, lane_count)).min(
            axis=1
        )
        expected_max = np.reshape(input, (ELEMENT_COUNT // lane_count, lane_count)).max(
            axis=1
        )
        return np.ravel(np.repeat(expected_min, expected_max), order="F")

    expected = wave_min_max(data, conditional, lane_count)
    # expected = np.interleave(expected_min, expected_max)

    ctx = helpers.dispatch_compute(
        device=device,
        path=Path(__file__).parent / "test_wave_ops.slang",
        entry_point="test_wave_min_max",
        thread_count=[ELEMENT_COUNT, 1, 1],
        buffers={
            "data": {"data": data},
            "result": {"element_count": ELEMENT_COUNT * 2},
        },
        params={"conditional": conditional},
    )

    result = ctx.buffers["result"].to_numpy().view(np.float32).flatten()
    print(expected)
    print(result)
    # print(result)
    assert np.all(result == expected)


if __name__ == "__main__":
    pytest.main([__file__, "-vvs", "-k", "test_wave_max"])
