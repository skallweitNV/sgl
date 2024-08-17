#include "vulkan_backend.h"
#include "vulkan_utils.h"

namespace sgl::rhi::vulkan {

HeapImpl::HeapImpl(ref<DeviceImpl> device, const HeapDesc& desc)
    : HeapBase(desc)
    , device(device)
{
}

HeapImpl::~HeapImpl() { }

NativeHandle HeapImpl::get_native_handle(NativeHandleType type) const
{
    return {};
}

ref<Heap> DeviceImpl::create_heap(const HeapDesc& desc)
{
    return ref(new HeapImpl(ref(this), desc));
}

} // namespace sgl::rhi::vulkan
