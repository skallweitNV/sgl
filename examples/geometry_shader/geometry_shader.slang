// SPDX-License-Identifier: Apache-2.0

import sgl.device.print;

RWByteAddressBuffer sum_buffer;

struct GSOut
{
    float4 pos : SV_POSITION;
    nointerpolation uint id : ID;
};

[shader("geometry")]
[maxvertexcount(3)]
void geometry_main(uint pid: SV_PrimitiveID, inout TriangleStream<GSOut> outStream)
{
    float theta = (pid / 16.f) * 6.28318530718;
    float2 center = float2(sin(theta), cos(theta)) * 0.5;

    GSOut gsout;
    gsout.id = pid;

    print("gs: pid = {}", pid);

    for (int i = 0; i < 3; ++i) {
        gsout.pos = float4(center + float2(i == 1 ? 0.1 : 0.0, i == 2 ? 0.1 : 0.0), 0.0, 1.0);
        // print("gs[{}] = {}", i, gsout.pos);
        outStream.Append(gsout);
    }

    // gsout.pos = float4(center + float2(0.1, 0.0), 0.0, 1.0);
    // outStream.Append(gsout);

    // gsout.pos = float4(center + float2(0.0, 0.1), 0.0, 1.0);
    // outStream.Append(gsout);

    outStream.RestartStrip();
}

[shader("vertex")]
void vertex_main()
{
}

[shader("fragment")]
void fragment_main(GSOut gsout) : SV_Target
{
    sum_buffer.InterlockedAdd(gsout.id * 4, 1);
}
