#include "vulkan_backend.h"
#include "vulkan_utils.h"

namespace sgl::rhi::vulkan {

VulkanAllocator::VulkanAllocator(VulkanContext& ctx)
    : m_ctx(ctx)
{
}

VulkanAllocator::~VulkanAllocator() { }

MemoryAllocation VulkanAllocator::allocate_memory(
    VkMemoryRequirements requirements,
    VkMemoryPropertyFlags flags,
    bool enable_device_address,
    bool enable_export_memory
)
{
    // find a memory space that satisfies the requirements
    VkPhysicalDeviceMemoryProperties memory_properties;
    m_ctx.api.vkGetPhysicalDeviceMemoryProperties(m_ctx.vk_physical_device, &memory_properties);

    uint32_t memory_type_index;
    for (memory_type_index = 0; memory_type_index < memory_properties.memoryTypeCount; memory_type_index++) {
        if ((requirements.memoryTypeBits & (1 << memory_type_index))
            && ((memory_properties.memoryTypes[memory_type_index].propertyFlags & flags) == flags)) {
            break;
        }
    }

    if (memory_type_index == memory_properties.memoryTypeCount)
        SGL_THROW("Failed to find a suitable memory type");

    VkMemoryAllocateInfo allocate_info = {VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    allocate_info.allocationSize = requirements.size;
    allocate_info.memoryTypeIndex = memory_type_index;

    VkMemoryAllocateFlagsInfo allocate_flags_info = {VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO};
    if (enable_device_address) {
        allocate_flags_info.flags |= VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT;
        utils::append_struct(allocate_info, allocate_flags_info);
    }

#if 0
    auto dedicatedAllocation
        = vk::MemoryDedicatedAllocateInfo().setImage(dedicatedImage).setBuffer(dedicatedBuffer).setPNext(pNext);

    if (dedicatedImage || dedicatedBuffer) {
        // Append the VkMemoryDedicatedAllocateInfo structure to the chain
        pNext = &dedicatedAllocation;
    }
#endif

    VkExportMemoryAllocateInfo export_info = {VK_STRUCTURE_TYPE_EXPORT_MEMORY_ALLOCATE_INFO};
    if (enable_export_memory) {
#if SGL_WINDOWS
        export_info.handleTypes = VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_WIN32_BIT;
#else
        export_info.handleTypes = VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_FD_BIT;
#endif
        utils::append_struct(allocate_info, export_info);
    }

    MemoryAllocation allocation;
    SGL_VK_CHECK(m_ctx.api.vkAllocateMemory(
        m_ctx.vk_device,
        &allocate_info,
        m_ctx.vk_allocation_callbacks,
        &allocation.vk_device_memory
    ));
    return allocation;
}

void VulkanAllocator::free_memory(MemoryAllocation& allocation)
{
    SGL_ASSERT(allocation.vk_device_memory != VK_NULL_HANDLE);
    m_ctx.api.vkFreeMemory(m_ctx.vk_device, allocation.vk_device_memory, m_ctx.vk_allocation_callbacks);
    allocation.vk_device_memory = VK_NULL_HANDLE;
}

} // namespace sgl::rhi::vulkan
