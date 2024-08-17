#include "vulkan_backend.h"
#include "vulkan_utils.h"

#include "sgl/core/type_utils.h"
#include "sgl/core/maths.h"

namespace sgl::rhi::vulkan {

BufferImpl::BufferImpl(ref<DeviceImpl> device, const BufferDesc& desc)
    : BufferBase(desc)
    , device(device)
{
    SGL_ASSERT(m_desc.size > 0);

    VkBufferUsageFlags usage_flags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    if (m_desc.is_vertex_buffer)
        usage_flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    if (m_desc.is_index_buffer)
        usage_flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

    VkBufferCreateInfo buffer_info = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    buffer_info.size = m_desc.size;
    buffer_info.usage = usage_flags;
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkExternalMemoryBufferCreateInfo external_create_info;
    if (m_desc.is_shared) {
#if SGL_WINDOWS
        external_create_info.handleTypes = VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_WIN32_BIT;
#else
        external_create_info.handleTypes = VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_FD_BIT;
#endif
        utils::append_struct(buffer_info, external_create_info);
    }

    VulkanContext& ctx = device->m_ctx;
    SGL_VK_CHECK(ctx.api.vkCreateBuffer(ctx.vk_device, &buffer_info, ctx.vk_allocation_callbacks, &vk_buffer));
    ctx.name_vk_object(vk_buffer, VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT, m_desc.debug_name.c_str());

    VulkanAllocator& allocator = device->m_allocator;
    VkMemoryRequirements requirements;
    ctx.api.vkGetBufferMemoryRequirements(ctx.vk_device, vk_buffer, &requirements);
    memory = allocator.allocate_memory(requirements, 0, true, m_desc.is_shared);
    ctx.name_vk_object(
        memory.vk_device_memory,
        VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT,
        m_desc.debug_name.c_str()
    );
    SGL_VK_CHECK(ctx.api.vkBindBufferMemory(ctx.vk_device, vk_buffer, memory.vk_device_memory, 0));

    if (ctx.api.vkGetBufferDeviceAddress) {
        VkBufferDeviceAddressInfo buffer_device_address_info = {VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO};
        buffer_device_address_info.buffer = vk_buffer;
        vk_device_address = ctx.api.vkGetBufferDeviceAddress(ctx.vk_device, &buffer_device_address_info);
    }

#if 0
    if (desc.sharedResourceFlags == SharedResourceFlags::Shared) {
#ifdef _WIN32
        buffer->sharedHandle = m_Context.device.getMemoryWin32HandleKHR(
            {buffer->memory, vk::ExternalMemoryHandleTypeFlagBits::eOpaqueWin32}
        );
#else
        buffer->sharedHandle = (void*)(size_t
        )m_Context.device.getMemoryFdKHR({buffer->memory, vk::ExternalMemoryHandleTypeFlagBits::eOpaqueFd});
#endif
#endif
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
    else if (type == NativeHandleType::vk_device_memory)
        return NativeHandle(memory.vk_device_memory);
    // else if (type == NativeHandleType::shared_handle)
    //     return NativeHandle(shared_handle);
    return {};
}

DeviceAddress BufferImpl::device_address() const
{
    return DeviceAddress(vk_device_address);
}

ref<Buffer> DeviceImpl::create_buffer(const BufferDesc& desc)
{
    return ref(new BufferImpl(ref(this), desc));
}

ref<Buffer> DeviceImpl::create_buffer_on_heap(const BufferDesc& desc, Heap* heap, uint64_t offset)
{
    ref<BufferImpl> buffer = ref(new BufferImpl(ref(this), desc));
    // TODO: bind heap memory
    return buffer;
}

SizeAndAlign DeviceImpl::get_buffer_size_and_align(const BufferDesc& desc)
{
    uint64_t align = 16; // TODO get from device
    uint64_t size = align_to(align, desc.size);
    return {size, align};
}

// SizeAndAlign DeviceImpl::get_buffer_size_and_align(Buffer* buffer_)
// {
//     BufferImpl* buffer = checked_cast<BufferImpl*>(buffer_);
//     VulkanContext& ctx = m_ctx;
//     VkMemoryRequirements vk_memory_requirements;
//     ctx.api.vkGetBufferMemoryRequirements(ctx.vk_device, buffer->vk_buffer, &vk_memory_requirements);
//     return SizeAndAlign{vk_memory_requirements.size, vk_memory_requirements.alignment};
// }

void* DeviceImpl::map_buffer(Buffer* buffer_, BufferRange range)
{
    BufferImpl* buffer = checked_cast<BufferImpl*>(buffer_);
}

void DeviceImpl::unmap_buffer(Buffer* buffer_)
{
    BufferImpl* buffer = checked_cast<BufferImpl*>(buffer_);
    // ctx.api.vkUnmapMemory(ctx.vk_device, buffer_impl->vk_memory);
}

} // namespace sgl::rhi::vulkan
