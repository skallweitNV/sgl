#pragma once

#include "metal_api.h"

#include "sgl/rhi/rhi.h"
#include "sgl/core/error.h"

namespace sgl::rhi::metal::utils {

// AdapterLUID get_adapter_luid(const VulkanApi& api, VkPhysicalDevice physical_device);

MTL::SamplerMinMagFilter translate_sampler_min_mag_filter(TextureFilteringMode mode);
MTL::SamplerMipFilter translate_sampler_mip_filter(TextureFilteringMode mode);
MTL::SamplerAddressMode translate_sampler_address_mode(TextureAddressingMode mode);
MTL::CompareFunction translate_compare_function(ComparisonFunc func);
// VkSamplerReductionMode translate_sampler_reduction_mode(TextureReductionOp op);

// #define SGL_VK_CHECK(call)                                                                                             \
//     {                                                                                                                  \
//         VkResult _result_ = call;                                                                                      \
//         if (_result_ != VK_SUCCESS)                                                                                    \
//             SGL_THROW("Vulkan error: {} failed with {}", #call, int(_result_));                                        \
//     }

inline NS::SharedPtr<NS::String> create_string(const char* str, NS::StringEncoding encoding = NS::UTF8StringEncoding)
{
    return NS::TransferPtr(NS::String::alloc()->init(str, encoding));
}

inline NS::SharedPtr<NS::String>
create_string_view(void* bytes, size_t len, NS::StringEncoding encoding = NS::UTF8StringEncoding)
{
    return NS::TransferPtr(NS::String::alloc()->init(bytes, len, encoding, false));
}

struct ScopedAutoreleasePool {
    ScopedAutoreleasePool() { m_pool = NS::AutoreleasePool::alloc()->init(); }
    ~ScopedAutoreleasePool() { m_pool->drain(); }
    NS::AutoreleasePool* m_pool;
};

#define AUTORELEASEPOOL ::sgl::rhi::metal::utils::ScopedAutoreleasePool _pool_;

} // namespace sgl::rhi::metal::utils
