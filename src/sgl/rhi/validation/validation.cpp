#include "validation.h"

namespace sgl::rhi::validation {

ref<Heap> DeviceValidator::create_heap(const HeapDesc& desc)
{
    return inner->create_heap(desc);
}

ref<Buffer> DeviceValidator::create_buffer(const BufferDesc& desc)
{
    return inner->create_buffer(desc);
}

ref<Buffer> DeviceValidator::create_buffer_on_heap(const BufferDesc& desc, Heap* heap, uint64_t offset)
{
    return inner->create_buffer_on_heap(desc, heap, offset);
}

SizeAndAlign DeviceValidator::get_buffer_size_and_align(const BufferDesc& desc)
{
    return inner->get_buffer_size_and_align(desc);
}

void* DeviceValidator::map_buffer(Buffer* buffer, BufferRange range)
{
    return inner->map_buffer(buffer, range);
}

void DeviceValidator::unmap_buffer(Buffer* buffer)
{
    inner->unmap_buffer(buffer);
}

ref<Texture> DeviceValidator::create_texture(const TextureDesc& desc)
{
    return inner->create_texture(desc);
}

ref<Texture> DeviceValidator::create_texture_on_heap(const TextureDesc& desc, Heap* heap, uint64_t offset)
{
    return inner->create_texture_on_heap(desc, heap, offset);
}

SizeAndAlign DeviceValidator::get_texture_size_and_align(const TextureDesc& desc)
{
    return inner->get_texture_size_and_align(desc);
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
