#include "metal_utils.h"

namespace sgl::rhi::metal::utils {

// AdapterLUID get_adapter_luid(const VulkanApi& api, VkPhysicalDevice physical_device)
// {
//     AdapterLUID luid = {};

//     VkPhysicalDeviceIDPropertiesKHR id_props = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ID_PROPERTIES_KHR};
//     VkPhysicalDeviceProperties2 props = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2};
//     props.pNext = &id_props;
//     SGL_ASSERT(api.vkGetPhysicalDeviceFeatures2);
//     api.vkGetPhysicalDeviceProperties2(physical_device, &props);
//     if (id_props.deviceLUIDValid) {
//         static_assert(sizeof(AdapterLUID) >= VK_LUID_SIZE);
//         ::memcpy(&luid, id_props.deviceLUID, VK_LUID_SIZE);
//     } else {
//         static_assert(sizeof(AdapterLUID) >= VK_UUID_SIZE);
//         ::memcpy(&luid, id_props.deviceUUID, VK_UUID_SIZE);
//     }

//     return luid;
// }

MTL::SamplerMinMagFilter translate_sampler_min_mag_filter(TextureFilteringMode mode)
{
    switch (mode) {
    case TextureFilteringMode::linear:
        return MTL::SamplerMinMagFilterLinear;
    case TextureFilteringMode::point:
        return MTL::SamplerMinMagFilterNearest;
    default:
        return MTL::SamplerMinMagFilter(0);
    }
}

MTL::SamplerMipFilter translate_sampler_mip_filter(TextureFilteringMode mode)
{
    switch (mode) {
    case TextureFilteringMode::linear:
        return MTL::SamplerMipFilterLinear;
    case TextureFilteringMode::point:
        return MTL::SamplerMipFilterNearest;
    default:
        return MTL::SamplerMipFilter(0);
    }
}

MTL::SamplerAddressMode translate_sampler_address_mode(TextureAddressingMode mode)
{
    switch (mode) {
    case TextureAddressingMode::wrap:
        return MTL::SamplerAddressModeRepeat;
    case TextureAddressingMode::clamp_to_edge:
        return MTL::SamplerAddressModeClampToEdge;
    case TextureAddressingMode::clamp_to_border:
        return MTL::SamplerAddressModeClampToBorderColor;
    case TextureAddressingMode::mirror_repeat:
        return MTL::SamplerAddressModeMirrorRepeat;
    case TextureAddressingMode::mirror_once:
        return MTL::SamplerAddressModeMirrorClampToEdge;
    default:
        return MTL::SamplerAddressMode(0);
    }
}

MTL::CompareFunction translate_compare_function(ComparisonFunc func)
{
    switch (func) {
    case ComparisonFunc::never:
        return MTL::CompareFunctionNever;
    case ComparisonFunc::less:
        return MTL::CompareFunctionLess;
    case ComparisonFunc::equal:
        return MTL::CompareFunctionEqual;
    case ComparisonFunc::less_equal:
        return MTL::CompareFunctionLessEqual;
    case ComparisonFunc::greater:
        return MTL::CompareFunctionGreater;
    case ComparisonFunc::not_equal:
        return MTL::CompareFunctionNotEqual;
    case ComparisonFunc::greater_equal:
        return MTL::CompareFunctionGreaterEqual;
    case ComparisonFunc::always:
        return MTL::CompareFunctionAlways;
    default:
        return MTL::CompareFunction(0);
    }
}

} // namespace sgl::rhi::metal::utils
