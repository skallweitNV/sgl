#include "validation.h"

namespace sgl::rhi::validation {

ref<Buffer> DeviceValidator::create_buffer(const BufferDesc& desc)
{
    return inner->create_buffer(desc);
}

ref<Texture> DeviceValidator::create_texture(const TextureDesc& desc)
{
    return inner->create_texture(desc);
}

ref<Sampler> DeviceValidator::create_sampler(const SamplerDesc& desc)
{
    return inner->create_sampler(desc);
}

void DeviceValidator::warn(const std::string_view msg)
{
    SGL_UNUSED(msg);
}

void DeviceValidator::error(const std::string_view msg)
{
    SGL_THROW("Validation error: {}", msg);
}

} // namespace sgl::rhi::validation
