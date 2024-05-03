import sgl

d = sgl.Device(type=sgl.DeviceType.vulkan)

a = d.create_buffer(size=4*1024*1024*1024, usage=sgl.ResourceUsage.unordered_access)

program = d.load_program("test.slang", ["main"])
k = d.create_compute_kernel(program)

for i in range(100):
    timer = sgl.Timer()
    k.dispatch(thread_count=[1, 1, 1], a=a)
    print(timer.elapsed_ms())

d.wait()
