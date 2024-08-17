#pragma once

// This needs to be included first, as it includes the Vulkan headers
// and sets the appropriate defines for each platform.
#include "vulkan_api.h"

#include "sgl/rhi/rhi.h"
#include "sgl/rhi/base.h"

namespace sgl::rhi::vulkan {

class AdapterImpl;
class DeviceImpl;

struct VulkanExtendedFeatures {
    // clang-format off

    // 16 bit storage features
    VkPhysicalDevice16BitStorageFeatures storage_16bit = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES_KHR};
    // Atomic Float features
    VkPhysicalDeviceShaderAtomicFloatFeaturesEXT shader_atomic_float = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_FLOAT_FEATURES_EXT};
    VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT shader_atomic_float2 = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_FLOAT_2_FEATURES_EXT};
    // Image int64 atomic features
    VkPhysicalDeviceShaderImageAtomicInt64FeaturesEXT shader_image_atomic_int64 = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_IMAGE_ATOMIC_INT64_FEATURES_EXT};
    // Extended dynamic state features
    VkPhysicalDeviceExtendedDynamicStateFeaturesEXT extended_dynamic_state = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT};
    // Acceleration structure features
    VkPhysicalDeviceAccelerationStructureFeaturesKHR acceleration_structure = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR};
    // Ray tracing pipeline features
    VkPhysicalDeviceRayTracingPipelineFeaturesKHR raytracing_pipeline = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR};
    // Ray query (inline ray-tracing) features
    VkPhysicalDeviceRayQueryFeaturesKHR ray_query = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_QUERY_FEATURES_KHR};
    // Raytracing invocation reorder (SER) features
    VkPhysicalDeviceRayTracingInvocationReorderFeaturesNV raytracing_invocation_reorder = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_INVOCATION_REORDER_FEATURES_NV};
    // Inline uniform block features
    VkPhysicalDeviceInlineUniformBlockFeaturesEXT inline_uniform_block = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_FEATURES_EXT};
    // Robustness2 features
    VkPhysicalDeviceRobustness2FeaturesEXT robustness2 = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT};
    // Variable pointer features
    VkPhysicalDeviceVariablePointerFeaturesKHR variable_pointer = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VARIABLE_POINTER_FEATURES_KHR};
    // Clock features
    VkPhysicalDeviceShaderClockFeaturesKHR clock = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CLOCK_FEATURES_KHR};
    // Mesh shader features
    VkPhysicalDeviceMeshShaderFeaturesEXT mesh_shader = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_EXT};
    // Multiview features
    VkPhysicalDeviceMultiviewFeaturesKHR multiview = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES_KHR};
    // Fragment shading rate features
    VkPhysicalDeviceFragmentShadingRateFeaturesKHR fragment_shading_rate = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_FEATURES_KHR};
    // Vulkan 1.2 features
    VkPhysicalDeviceVulkan12Features vulkan_12 = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES};
    // Custom border color features
    VkPhysicalDeviceCustomBorderColorFeaturesEXT custom_border_color = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_FEATURES_EXT};

    // clang-format on
};

struct VulkanContext {
    VulkanModule module;
    VulkanApi api;

    VkInstance vk_instance{VK_NULL_HANDLE};
    VkPhysicalDevice vk_physical_device{VK_NULL_HANDLE};
    VkDevice vk_device{VK_NULL_HANDLE};
    VkAllocationCallbacks* vk_allocation_callbacks{nullptr};

    VulkanExtendedFeatures extended_features;

    VkPhysicalDeviceProperties physical_device_properties;
    VkPhysicalDeviceFeatures physical_device_features;
    VkPhysicalDeviceMemoryProperties physical_device_memory_properties;

    void name_vk_object(void* object, VkDebugReportObjectTypeEXT object_type, const char* name) const;
};

struct MemoryAllocation {
    VkDeviceMemory vk_device_memory{VK_NULL_HANDLE};
};

class VulkanAllocator {
public:
    VulkanAllocator(VulkanContext& ctx);
    ~VulkanAllocator();

    MemoryAllocation allocate_memory(
        VkMemoryRequirements requirements,
        VkMemoryPropertyFlags flags,
        bool enable_device_address = false,
        bool enable_export_memory = false
    );
    void free_memory(MemoryAllocation& allocation);

private:
    VulkanContext& m_ctx;
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
    // VkBuffer vk_buffer{VK_NULL_HANDLE};
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
    VkBuffer vk_buffer{VK_NULL_HANDLE};
    VkDeviceAddress vk_device_address{0};
    MemoryAllocation memory;
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
    VkImage vk_image{VK_NULL_HANDLE};
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
    VkSampler vk_sampler{VK_NULL_HANDLE};
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

    VulkanContext m_ctx;
    VulkanAllocator m_allocator;

    VkDebugReportCallbackEXT m_vk_debug_report_callback{VK_NULL_HANDLE};

private:
    void initialize_instance_and_device(
        VkInstance instance,
        VkPhysicalDevice physical_device,
        VkDevice device,
        AdapterImpl* adapter,
        bool enable_validation_layer
    );
};

// ----------------------------------------------------------------------------
// AdapterImpl
// ----------------------------------------------------------------------------

class AdapterImpl : public AdapterBase {
public:
    virtual ref<Device> create_device(const DeviceDesc& desc) override;
};

} // namespace sgl::rhi::vulkan
