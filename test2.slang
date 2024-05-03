struct CB {
    int a;
    int b;
    int c;
};

// cbuffer CB cb;

struct PB {
    float a;
    float b;
    float c;
    StructuredBuffer<uint> ba;
    RWBuffer<float> bb;
    Texture2D<float> t;

    void work(uint tid) {
        bb[tid] = float(ba[tid] * a + b + c);
    }
};

ParameterBlock<CB> cb;
ParameterBlock<PB> pb1;
ParameterBlock<PB> pb2;

[shader("compute")]
[numthreads(1, 1, 1)]
void main(uint tid: SV_DispatchThreadID, uniform PB pb3)
{
    pb1.work(tid);
    pb2.work(tid);
    pb3.work(tid);
}
