[shader("compute")]
[numthreads(1, 1, 1)]
void main(uint tid: SV_DispatchThreadID, RWStructuredBuffer<uint> a)
{
    a[tid] = tid;
}
