#include "vulkan_backend.h"
#include "vulkan_utils.h"

namespace sgl::rhi::vulkan {

BufferImpl::BufferImpl(ref<DeviceImpl> device, const BufferDesc& desc)
    : BufferBase(desc)
    , device(device)
{
    VkBufferCreateInfo buffer_info = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};

    const auto& ctx = device->m_ctx;
    SGL_VK_CHECK(ctx.api.vkCreateBuffer(ctx.vk_device, &buffer_info, ctx.vk_allocation_callbacks, &vk_buffer));
}

BufferImpl::~BufferImpl()
{
    const auto& ctx = device->m_ctx;
    ctx.api.vkDestroyBuffer(ctx.vk_device, vk_buffer, ctx.vk_allocation_callbacks);
}

NativeHandle BufferImpl::get_native_handle(NativeHandleType type) const
{
    if (type == NativeHandleType::vk_buffer)
        return NativeHandle(vk_buffer);
    return {};
}

DeviceAddress BufferImpl::device_address() const
{
    SGL_UNIMPLEMENTED();
    return 0;
}

ref<Buffer> DeviceImpl::create_buffer(const BufferDesc& desc)
{
    return ref(new BufferImpl(ref(this), desc));
}

} // namespace sgl::rhi::vulkan
