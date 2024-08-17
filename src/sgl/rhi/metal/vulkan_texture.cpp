#include "vulkan_backend.h"
#include "vulkan_utils.h"

namespace sgl::rhi::vulkan {

TextureImpl::TextureImpl(ref<DeviceImpl> device, const TextureDesc& desc)
    : TextureBase(desc)
    , device(device)
{
    VkImageCreateInfo image_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};

    const auto& ctx = device->m_ctx;
    SGL_VK_CHECK(ctx.api.vkCreateImage(ctx.vk_device, &image_info, ctx.vk_allocation_callbacks, &vk_image));
}

TextureImpl::~TextureImpl()
{
    const auto& ctx = device->m_ctx;
    ctx.api.vkDestroyImage(ctx.vk_device, vk_image, ctx.vk_allocation_callbacks);
}

NativeHandle TextureImpl::get_native_handle(NativeHandleType type) const
{
    if (type == NativeHandleType::vk_image)
        return NativeHandle(vk_image);
    return {};
}

DeviceAddress TextureImpl::device_address() const
{
    SGL_UNIMPLEMENTED();
    return 0;
}

ref<Texture> DeviceImpl::create_texture(const TextureDesc& desc)
{
    return ref(new TextureImpl(ref(this), desc));
}

ref<Texture> DeviceImpl::create_texture_on_heap(const TextureDesc& desc, Heap* heap, uint64_t offset) { }

SizeAndAlign DeviceImpl::get_texture_size_and_align(const TextureDesc& desc) { }

} // namespace sgl::rhi::vulkan
