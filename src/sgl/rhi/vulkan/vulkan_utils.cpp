#include "vulkan_utils.h"

namespace sgl::rhi::vulkan::utils {

AdapterLUID get_adapter_luid(const VulkanApi& api, VkPhysicalDevice physical_device)
{
    AdapterLUID luid = {};

    VkPhysicalDeviceIDPropertiesKHR id_props = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ID_PROPERTIES_KHR};
    VkPhysicalDeviceProperties2 props = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2};
    props.pNext = &id_props;
    SGL_ASSERT(api.vkGetPhysicalDeviceFeatures2);
    api.vkGetPhysicalDeviceProperties2(physical_device, &props);
    if (id_props.deviceLUIDValid) {
        static_assert(sizeof(AdapterLUID) >= VK_LUID_SIZE);
        ::memcpy(&luid, id_props.deviceLUID, VK_LUID_SIZE);
    } else {
        static_assert(sizeof(AdapterLUID) >= VK_UUID_SIZE);
        ::memcpy(&luid, id_props.deviceUUID, VK_UUID_SIZE);
    }

    return luid;
}

VkFilter translate_filter(TextureFilteringMode mode)
{
    switch (mode) {
    case TextureFilteringMode::linear:
        return VK_FILTER_LINEAR;
    case TextureFilteringMode::point:
        return VK_FILTER_NEAREST;
    default:
        return VkFilter(0);
    }
}

VkSamplerMipmapMode translate_sampler_mipmap_mode(TextureFilteringMode mode)
{
    switch (mode) {
    case TextureFilteringMode::linear:
        return VK_SAMPLER_MIPMAP_MODE_LINEAR;
    case TextureFilteringMode::point:
        return VK_SAMPLER_MIPMAP_MODE_NEAREST;
    default:
        return VkSamplerMipmapMode(0);
    }
}

VkSamplerAddressMode translate_sampler_address_mode(TextureAddressingMode mode)
{
    switch (mode) {
    case TextureAddressingMode::wrap:
        return VK_SAMPLER_ADDRESS_MODE_REPEAT;
    case TextureAddressingMode::clamp_to_edge:
        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    case TextureAddressingMode::clamp_to_border:
        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    case TextureAddressingMode::mirror_repeat:
        return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
    case TextureAddressingMode::mirror_once:
        return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
    default:
        return VkSamplerAddressMode(0);
    }
}

VkCompareOp translate_compare_op(ComparisonFunc func)
{
    switch (func) {
    case ComparisonFunc::never:
        return VK_COMPARE_OP_NEVER;
    case ComparisonFunc::less:
        return VK_COMPARE_OP_LESS;
    case ComparisonFunc::equal:
        return VK_COMPARE_OP_EQUAL;
    case ComparisonFunc::less_equal:
        return VK_COMPARE_OP_LESS_OR_EQUAL;
    case ComparisonFunc::greater:
        return VK_COMPARE_OP_GREATER;
    case ComparisonFunc::not_equal:
        return VK_COMPARE_OP_NOT_EQUAL;
    case ComparisonFunc::greater_equal:
        return VK_COMPARE_OP_GREATER_OR_EQUAL;
    case ComparisonFunc::always:
        return VK_COMPARE_OP_ALWAYS;
    default:
        return VkCompareOp(0);
    }
}

VkSamplerReductionMode translate_sampler_reduction_mode(TextureReductionOp op)
{
    switch (op) {
    case TextureReductionOp::minimum:
        return VK_SAMPLER_REDUCTION_MODE_MIN;
    case TextureReductionOp::maximum:
        return VK_SAMPLER_REDUCTION_MODE_MAX;
    default:
        return VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE;
    }
}

} // namespace sgl::rhi::vulkan::utils
