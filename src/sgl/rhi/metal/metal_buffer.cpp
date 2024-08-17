#include "metal_backend.h"
#include "metal_utils.h"

#include "sgl/core/type_utils.h"
#include "sgl/core/maths.h"

namespace sgl::rhi::metal {

BufferImpl::BufferImpl(ref<DeviceImpl> device, const BufferDesc& desc)
    : BufferBase(desc)
    , device(device)
{
}

BufferImpl::~BufferImpl() { }

NativeHandle BufferImpl::get_native_handle(NativeHandleType type) const
{
    if (type == NativeHandleType::mtl_buffer)
        return NativeHandle(mtl_buffer.get());
    return {};
}

DeviceAddress BufferImpl::device_address() const
{
    return mtl_buffer->gpuAddress();
}

ref<Buffer> DeviceImpl::create_buffer(const BufferDesc& desc)
{
    AUTORELEASEPOOL
    return ref(new BufferImpl(ref(this), desc));
}

ref<Buffer> DeviceImpl::create_buffer_on_heap(const BufferDesc& desc, Heap* heap, uint64_t offset)
{
    AUTORELEASEPOOL
    ref<BufferImpl> buffer = ref(new BufferImpl(ref(this), desc));
    // TODO: bind heap memory
    return buffer;
}

SizeAndAlign DeviceImpl::get_buffer_size_and_align(const BufferDesc& desc)
{
    // TODO memory type
    MTL::SizeAndAlign size_and_align = m_ctx.device->heapBufferSizeAndAlign(desc.size, 0);
    return {size_and_align.size, size_and_align.align};
}

void* DeviceImpl::map_buffer(Buffer* buffer_, BufferRange range)
{
    BufferImpl* buffer = checked_cast<BufferImpl*>(buffer_);
    return reinterpret_cast<uint8_t*>(buffer->mtl_buffer->contents()) + range.offset;
}

void DeviceImpl::unmap_buffer(Buffer* buffer_)
{
    BufferImpl* buffer = checked_cast<BufferImpl*>(buffer_);
}

} // namespace sgl::rhi::metal
