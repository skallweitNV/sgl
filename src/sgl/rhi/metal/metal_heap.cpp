#include "metal_backend.h"
#include "metal_utils.h"

namespace sgl::rhi::metal {

HeapImpl::HeapImpl(ref<DeviceImpl> device, const HeapDesc& desc)
    : HeapBase(desc)
    , device(device)
{
    NS::SharedPtr<MTL::HeapDescriptor> heap_desc = NS::TransferPtr(MTL::HeapDescriptor::alloc()->init());
    heap_desc->setSize(desc.size);

    mtl_heap = NS::TransferPtr(device->m_ctx.device->newHeap(heap_desc.get()));
    SGL_CHECK(mtl_heap, "Failed to create heap");
}

HeapImpl::~HeapImpl() { }

NativeHandle HeapImpl::get_native_handle(NativeHandleType type) const
{
    if (type == NativeHandleType::mtl_heap)
        return NativeHandle(mtl_heap.get());
    return {};
}

ref<Heap> DeviceImpl::create_heap(const HeapDesc& desc)
{
    AUTORELEASEPOOL
    return ref(new HeapImpl(ref(this), desc));
}

} // namespace sgl::rhi::metal
