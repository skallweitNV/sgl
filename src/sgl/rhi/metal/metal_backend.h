#pragma once

#include "metal_api.h"

#include "sgl/rhi/rhi.h"
#include "sgl/rhi/base.h"

namespace sgl::rhi::metal {

class AdapterImpl;
class DeviceImpl;

struct MetalContext {
    NS::SharedPtr<MTL::Device> device;
    // void name_vk_object(void* object, VkDebugReportObjectTypeEXT object_type, const char* name) const;
};

struct MemoryAllocation {
    // VkDeviceMemory vk_device_memory{VK_NULL_HANDLE};
};

// ----------------------------------------------------------------------------
// HeapImpl
// ----------------------------------------------------------------------------

class HeapImpl : public HeapBase {
public:
    HeapImpl(ref<DeviceImpl> device, const HeapDesc& desc);
    virtual ~HeapImpl();

    virtual NativeHandle get_native_handle(NativeHandleType type) const override;

    ref<DeviceImpl> device;
    NS::SharedPtr<MTL::Heap> mtl_heap;
};

// ----------------------------------------------------------------------------
// BufferImpl
// ----------------------------------------------------------------------------

class BufferImpl : public BufferBase {
public:
    BufferImpl(ref<DeviceImpl> device, const BufferDesc& desc);
    virtual ~BufferImpl();

    virtual NativeHandle get_native_handle(NativeHandleType type) const override;

    virtual DeviceAddress device_address() const override;

    ref<DeviceImpl> device;
    ref<HeapImpl> heap;
    NS::SharedPtr<MTL::Buffer> mtl_buffer;
};

// ----------------------------------------------------------------------------
// TextureImpl
// ----------------------------------------------------------------------------

class TextureImpl : public TextureBase {
public:
    TextureImpl(ref<DeviceImpl> device, const TextureDesc& desc);
    virtual ~TextureImpl();

    virtual NativeHandle get_native_handle(NativeHandleType type) const override;

    virtual DeviceAddress device_address() const override;

    ref<DeviceImpl> device;
    ref<HeapImpl> heap;
    NS::SharedPtr<MTL::Texture> mtl_texture;
};

// ----------------------------------------------------------------------------
// SamplerImpl
// ----------------------------------------------------------------------------

class SamplerImpl : public SamplerBase {
public:
    SamplerImpl(ref<DeviceImpl> device, const SamplerDesc& desc);
    virtual ~SamplerImpl();

    virtual NativeHandle get_native_handle(NativeHandleType type) const override;

    ref<DeviceImpl> device;
    NS::SharedPtr<MTL::SamplerState> mtl_sampler_state;
};

// ----------------------------------------------------------------------------
// DeviceImpl
// ----------------------------------------------------------------------------

class DeviceImpl : public DeviceBase {
public:
    DeviceImpl(const DeviceDesc& desc, AdapterImpl* adapter);
    virtual ~DeviceImpl();

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

    MetalContext m_ctx;
};

// ----------------------------------------------------------------------------
// AdapterImpl
// ----------------------------------------------------------------------------

class AdapterImpl : public AdapterBase {
public:
    virtual ref<Device> create_device(const DeviceDesc& desc) override;
};

} // namespace sgl::rhi::metal
