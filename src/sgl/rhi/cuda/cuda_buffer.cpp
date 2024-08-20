#include "cuda_backend.h"
#include "cuda_utils.h"

#include "sgl/core/type_utils.h"
#include "sgl/core/maths.h"

namespace sgl::rhi::cuda {

BufferImpl::BufferImpl(ref<DeviceImpl> device, const BufferDesc& desc)
    : BufferBase(desc)
    , device(device)
{
}

BufferImpl::~BufferImpl() {
    switch (m_desc.memory_type) {
    case MemoryType::device_local:
        SGL_CU_CHECK(cuMemFree(cu_device_ptr));
        break;
    case MemoryType::upload:
    case MemoryType::read_back:
        SGL_CU_CHECK(cuMemFreeHost(cu_host_ptr));
        break;
    }
}

bool BufferImpl::init()
{
    if (m_desc.size == 0)
        return false;

    switch (m_desc.memory_type) {
    case MemoryType::device_local:
        SGL_CU_CHECK(cuMemAlloc(&cu_device_ptr, m_desc.size));
        break;
    case MemoryType::upload:
    case MemoryType::read_back:
        SGL_CU_CHECK(cuMemAllocHost(&cu_host_ptr, m_desc.size));
        break;
    }
    return true;
}

NativeHandle BufferImpl::get_native_handle(NativeHandleType type) const
{
    if (type == NativeHandleType::cu_device_ptr)
        return NativeHandle(cu_device_ptr);
    return {};
}

DeviceAddress BufferImpl::device_address() const
{
    return DeviceAddress(cu_device_ptr);
}

ref<Buffer> DeviceImpl::create_buffer(const BufferDesc& desc)
{
    ref<BufferImpl> buffer = ref(new BufferImpl(ref(this), desc));
    if (!buffer->init())
        return nullptr;
    return buffer;
}

ref<Buffer> DeviceImpl::create_buffer_on_heap(const BufferDesc& desc, Heap* heap, uint64_t offset)
{
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
    if (buffer->m_desc.memory_type == MemoryType::device_local)
        return nullptr;
    return reinterpret_cast<uint8_t*>(buffer->cu_host_ptr) + range.offset;
}

void DeviceImpl::unmap_buffer(Buffer* buffer_)
{
    BufferImpl* buffer = checked_cast<BufferImpl*>(buffer_);
}

} // namespace sgl::rhi::cuda
