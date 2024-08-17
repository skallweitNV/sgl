#include "metal_backend.h"
#include "metal_utils.h"

namespace sgl::rhi::metal {

TextureImpl::TextureImpl(ref<DeviceImpl> device, const TextureDesc& desc)
    : TextureBase(desc)
    , device(device)
{
}

TextureImpl::~TextureImpl() { }

NativeHandle TextureImpl::get_native_handle(NativeHandleType type) const
{
    if (type == NativeHandleType::mtl_texture)
        return NativeHandle(mtl_texture.get());
    return {};
}

DeviceAddress TextureImpl::device_address() const
{
    SGL_UNIMPLEMENTED();
    return 0;
}

ref<Texture> DeviceImpl::create_texture(const TextureDesc& desc)
{
    AUTORELEASEPOOL
    return ref(new TextureImpl(ref(this), desc));
}

ref<Texture> DeviceImpl::create_texture_on_heap(const TextureDesc& desc, Heap* heap, uint64_t offset)
{
    AUTORELEASEPOOL
    return {};
}

SizeAndAlign DeviceImpl::get_texture_size_and_align(const TextureDesc& desc)
{
    NS::SharedPtr<MTL::TextureDescriptor> texture_desc = NS::TransferPtr(MTL::TextureDescriptor::alloc()->init());
    MTL::SizeAndAlign size_and_align = m_ctx.device->heapTextureSizeAndAlign(texture_desc.get());
    return {size_and_align.size, size_and_align.align};
}

} // namespace sgl::rhi::metal
