

[numthreads(1, 1, 1)]
void query_lane_count(uint tid: SV_DispatchThreadID, RWStructuredBuffer<uint> result)
{
    result[0] = WaveGetLaneCount();
}

[shader("compute")]
[numthreads(128, 1, 1)]
void test_wave_match(uint tid: SV_DispatchThreadID, StructuredBuffer<uint> data, RWStructuredBuffer<uint4> result)
{
    uint value = data[tid];
    result[tid] = WaveMatch(value);
}

[shader("compute")]
[numthreads(128, 1, 1)]
void test_wave_max_int(uint tid: SV_DispatchThreadID, StructuredBuffer<int> data, RWStructuredBuffer<int> result)
{
    int value = data[tid];
    if (value < 0)
        value = WaveActiveMax(value);
    result[tid] = value;
}

[shader("compute")]
[numthreads(128, 1, 1)]
void test_wave_max_float(uint tid: SV_DispatchThreadID, StructuredBuffer<float> data, RWStructuredBuffer<float> result)
{
    float value = data[tid];
    if (value < 0)
        value = WaveActiveMax(value);
    result[tid] = value;
}

[shader("compute")]
[numthreads(128, 1, 1)]
void test_wave_min_max_float(
    uint tid: SV_DispatchThreadID,
    StructuredBuffer<float> data,
    RWStructuredBuffer<float> result,
    uniform bool conditional
)
{
    float value = asfloat(data[tid]);

    float min_value = 0.f;
    float max_value = 0.f;

    if (conditional) {
        if (value - floor(value) < 0.5f) {
            min_value = WaveActiveMin(value);
            max_value = WaveActiveMax(value);
        }
    } else {
        min_value = WaveActiveMin(value);
        max_value = WaveActiveMax(value);
    }

    result[tid * 2 + 0] = min_value;
    result[tid * 2 + 1] = max_value;
}
