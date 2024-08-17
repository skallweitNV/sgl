import sgl
from sgl import rhi

print(rhi.FormatInfo.get(rhi.Format.rgb32_float))

# device = sgl.Device()
# p = device.load_program(module_name="C:/projects/sgl/test.slang", entry_point_names=["testInitializers"], link_options={"dump_intermediates": True})


# range = sgl.rhi.BufferRange(100)
# print(range.size, range.offset)

adapters=sgl.rhi.enum_adapters()
print(adapters)

adapter = rhi.default_adapter(api=rhi.GraphicsAPI.metal)
print(adapter)
print(adapter.info)


device = rhi.create_device(enable_validation=True, adapter=adapter)
print(device)
print(device.info)

sampler = device.create_sampler()
print(sampler)


desc = rhi.BufferDesc()
desc.size = 1024
buffer = device.create_buffer(desc)
print(buffer)
req = device.get_buffer_size_and_align(desc)
print(req.size, req.align)
