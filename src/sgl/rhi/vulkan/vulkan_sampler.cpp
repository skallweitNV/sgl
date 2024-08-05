#include "vulkan_backend.h"
#include "vulkan_utils.h"

namespace sgl::rhi::vulkan {

SamplerImpl::SamplerImpl(ref<DeviceImpl> device, const SamplerDesc& desc)
    : SamplerBase(desc)
    , device(device)
{
    VkSamplerCreateInfo sampler_info = {VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};
    sampler_info.magFilter = utils::translate_filter(desc.mag_filter);
    sampler_info.minFilter = utils::translate_filter(desc.min_filter);
    sampler_info.mipmapMode = utils::translate_sampler_mipmap_mode(desc.mip_filter);
    sampler_info.addressModeU = utils::translate_sampler_address_mode(desc.address_u);
    sampler_info.addressModeV = utils::translate_sampler_address_mode(desc.address_v);
    sampler_info.addressModeW = utils::translate_sampler_address_mode(desc.address_w);
    sampler_info.mipLodBias = desc.mip_bias;
    sampler_info.anisotropyEnable = desc.max_anisotropy > 1;
    sampler_info.maxAnisotropy = float(desc.max_anisotropy);
    sampler_info.compareEnable = desc.reduction_op != TextureReductionOp::comparison;
    sampler_info.compareOp = utils::translate_compare_op(desc.comparison_func);
    sampler_info.minLod = std::max(0.f, desc.mip_min);
    sampler_info.maxLod = std::clamp(desc.mip_max, sampler_info.minLod, VK_LOD_CLAMP_NONE);
    sampler_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    sampler_info.unnormalizedCoordinates = VK_FALSE;

    VkSamplerReductionModeCreateInfo reduction_info = {VK_STRUCTURE_TYPE_SAMPLER_REDUCTION_MODE_CREATE_INFO};
    reduction_info.reductionMode = utils::translate_sampler_reduction_mode(desc.reduction_op);
    utils::append_struct(sampler_info, reduction_info);

    const auto& ctx = device->m_ctx;

    VkSamplerCustomBorderColorCreateInfoEXT border_color_info
        = {VK_STRUCTURE_TYPE_SAMPLER_CUSTOM_BORDER_COLOR_CREATE_INFO_EXT};
    if (ctx.extended_features.custom_border_color.customBorderColors) {
        border_color_info.customBorderColor.float32[0] = desc.border_color.r;
        border_color_info.customBorderColor.float32[1] = desc.border_color.g;
        border_color_info.customBorderColor.float32[2] = desc.border_color.b;
        border_color_info.customBorderColor.float32[3] = desc.border_color.a;
        sampler_info.borderColor = VK_BORDER_COLOR_FLOAT_CUSTOM_EXT;
        utils::append_struct(sampler_info, border_color_info);
    }

    SGL_VK_CHECK(ctx.api.vkCreateSampler(ctx.vk_device, &sampler_info, ctx.vk_allocation_callbacks, &vk_sampler));
}

SamplerImpl::~SamplerImpl()
{
    const auto& ctx = device->m_ctx;
    ctx.api.vkDestroySampler(ctx.vk_device, vk_sampler, ctx.vk_allocation_callbacks);
}

NativeHandle SamplerImpl::get_native_handle(NativeHandleType type) const
{
    if (type == NativeHandleType::vk_sampler)
        return NativeHandle(vk_sampler);
    return {};
}

ref<Sampler> DeviceImpl::create_sampler(const SamplerDesc& desc)
{
    return ref(new SamplerImpl(ref(this), desc));
}

} // namespace sgl::rhi::vulkan
