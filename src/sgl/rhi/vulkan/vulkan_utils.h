#pragma once

#include "vulkan_api.h"

#include "sgl/rhi/rhi.h"
#include "sgl/core/error.h"

namespace sgl::rhi::vulkan::utils {

AdapterLUID get_adapter_luid(const VulkanApi& api, VkPhysicalDevice physical_device);

VkFilter translate_filter(TextureFilteringMode mode);
VkSamplerMipmapMode translate_sampler_mipmap_mode(TextureFilteringMode mode);
VkSamplerAddressMode translate_sampler_address_mode(TextureAddressingMode mode);
VkCompareOp translate_compare_op(ComparisonFunc func);
VkSamplerReductionMode translate_sampler_reduction_mode(TextureReductionOp op);

template<typename A, typename B>
void append_struct(A& head, B& item)
{
    item.pNext = head.pNext;
    head.pNext = &item;
}

#define SGL_VK_CHECK(call)                                                                                             \
    {                                                                                                                  \
        VkResult _result_ = call;                                                                                      \
        if (_result_ != VK_SUCCESS)                                                                                    \
            SGL_THROW("Vulkan error: {} failed with {}", #call, int(_result_));                                        \
    }

} // namespace sgl::rhi::vulkan::utils
