#pragma once

#include "sgl/rhi/rhi.h"

#include "sgl/core/format.h"

namespace sgl::rhi::validation {

class DeviceValidator : public Device {
public:
    DeviceValidator(ref<Device> inner)
        : inner(inner)
    {
    }
    ~DeviceValidator() { }

    virtual const DeviceDesc& desc() const override { return inner->desc(); }
    virtual const DeviceInfo& info() const override { return inner->info(); }

    virtual ref<Heap> create_heap(const HeapDesc& desc) override;
    virtual ref<Buffer> create_buffer(const BufferDesc& desc) override;
    virtual ref<Buffer> create_buffer_on_heap(const BufferDesc& desc, Heap* heap, uint64_t offset) override;
    virtual SizeAndAlign get_buffer_size_and_align(const BufferDesc& desc) override;
    virtual void* map_buffer(Buffer* buffer, BufferRange range) override;
    virtual void unmap_buffer(Buffer* buffer) override;
    virtual ref<Texture> create_texture(const TextureDesc& desc) override;
    virtual ref<Texture> create_texture_on_heap(const TextureDesc& desc, Heap* heap, uint64_t offset) override;
    virtual SizeAndAlign get_texture_size_and_align(const TextureDesc& desc) override;
    virtual ref<Sampler> create_sampler(const SamplerDesc& desc) override;

    virtual std::string to_string() const override { return inner->to_string(); }

    ref<Device> inner;

private:
    void warn(const std::string_view msg);
    void error(const std::string_view msg);

    template<typename... Args>
    inline void warn(fmt::format_string<Args...> fmt, Args&&... args)
    {
        warn(fmt::format(fmt, std::forward<Args>(args)...));
    }

    template<typename... Args>
    inline void error(fmt::format_string<Args...> fmt, Args&&... args)
    {
        error(fmt::format(fmt, std::forward<Args>(args)...));
    }
};

inline ref<Device> wrap_device(ref<Device> device)
{
    return ref<Device>(new DeviceValidator(device));
}

} // namespace sgl::rhi::validation
