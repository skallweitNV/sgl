#pragma once

#include "sgl/core/macros.h"
#include "sgl/core/platform.h"

#if SGL_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR 1
#elif SGL_LINUX
#define VK_USE_PLATFORM_XLIB_KHR 1
#elif SGL_MACOS
#define VK_USE_PLATFORM_METAL_EXT 1
#else
#error "Platform not supported"
#endif

#define VK_NO_PROTOTYPES

#include <vulkan/vulkan.h>

// Undef xlib macros
#ifdef Always
#undef Always
#endif
#ifdef None
#undef None
#endif

namespace sgl::rhi::vulkan {

// ----------------------------------------------------------------------------
// VulkanModule
// ----------------------------------------------------------------------------

class VulkanModule {
public:
    ~VulkanModule() { unload(); }

    bool load();
    void unload();

    bool is_available() const { return m_module != nullptr; }

    PFN_vkVoidFunction get_function(const char* name) const;

private:
    SharedLibraryHandle m_module = nullptr;
};

// clang-format off

#define VULKAN_API_GLOBAL_PROCS(x) \
    x(vkGetInstanceProcAddr) \
    x(vkCreateInstance) \
    x(vkEnumerateInstanceLayerProperties) \
    x(vkEnumerateDeviceExtensionProperties) \
    x(vkDestroyInstance) \
    /* */

#define VULKAN_API_INSTANCE_PROCS_OPT(x) \
    x(vkGetPhysicalDeviceFeatures2) \
    x(vkGetPhysicalDeviceProperties2) \
    x(vkCreateDebugReportCallbackEXT) \
    x(vkDestroyDebugReportCallbackEXT) \
    x(vkDebugReportMessageEXT) \
    /* */

#define VULKAN_API_INSTANCE_PROCS(x) \
    x(vkCreateDevice) \
    x(vkDestroyDevice) \
    x(vkEnumeratePhysicalDevices) \
    x(vkGetPhysicalDeviceProperties) \
    x(vkGetPhysicalDeviceFeatures) \
    x(vkGetPhysicalDeviceMemoryProperties) \
    x(vkGetPhysicalDeviceQueueFamilyProperties) \
    x(vkGetPhysicalDeviceFormatProperties) \
    x(vkGetDeviceProcAddr) \
    /* */

#define VULKAN_API_DEVICE_PROCS(x) \
    x(vkCreateDescriptorPool) \
    x(vkDestroyDescriptorPool) \
    x(vkResetDescriptorPool) \
    x(vkGetDeviceQueue) \
    x(vkQueueSubmit) \
    x(vkQueueWaitIdle) \
    x(vkCreateBuffer) \
    x(vkAllocateMemory) \
    x(vkMapMemory) \
    x(vkUnmapMemory) \
    x(vkCmdCopyBuffer) \
    x(vkDestroyBuffer) \
    x(vkFreeMemory) \
    x(vkCreateDescriptorSetLayout) \
    x(vkDestroyDescriptorSetLayout) \
    x(vkAllocateDescriptorSets) \
    x(vkFreeDescriptorSets) \
    x(vkUpdateDescriptorSets) \
    x(vkCreatePipelineLayout) \
    x(vkDestroyPipelineLayout) \
    x(vkCreateComputePipelines) \
    x(vkCreateGraphicsPipelines) \
    x(vkDestroyPipeline) \
    x(vkCreateShaderModule) \
    x(vkDestroyShaderModule) \
    x(vkCreateFramebuffer) \
    x(vkDestroyFramebuffer) \
    x(vkCreateImage) \
    x(vkDestroyImage) \
    x(vkCreateImageView) \
    x(vkDestroyImageView) \
    x(vkCreateRenderPass) \
    x(vkDestroyRenderPass) \
    x(vkCreateCommandPool) \
    x(vkDestroyCommandPool) \
    x(vkCreateSampler) \
    x(vkDestroySampler) \
    x(vkCreateBufferView) \
    x(vkDestroyBufferView) \
    \
    x(vkGetBufferMemoryRequirements) \
    x(vkGetImageMemoryRequirements) \
    \
    x(vkCmdBindPipeline) \
    x(vkCmdClearAttachments) \
    x(vkCmdClearColorImage) \
    x(vkCmdClearDepthStencilImage) \
    x(vkCmdFillBuffer) \
    x(vkCmdBindDescriptorSets) \
    x(vkCmdDispatch) \
    x(vkCmdDraw) \
    x(vkCmdDrawIndexed) \
    x(vkCmdDrawIndirect) \
    x(vkCmdDrawIndexedIndirect) \
    x(vkCmdSetScissor) \
    x(vkCmdSetViewport) \
    x(vkCmdBindVertexBuffers) \
    x(vkCmdBindIndexBuffer) \
    x(vkCmdBeginRenderPass) \
    x(vkCmdEndRenderPass) \
    x(vkCmdPipelineBarrier) \
    x(vkCmdCopyBufferToImage)\
    x(vkCmdCopyImage) \
    x(vkCmdCopyImageToBuffer) \
    x(vkCmdResolveImage) \
    x(vkCmdPushConstants) \
    x(vkCmdSetStencilReference) \
    x(vkCmdWriteTimestamp) \
    x(vkCmdBeginQuery) \
    x(vkCmdEndQuery) \
    x(vkCmdResetQueryPool) \
    x(vkCmdCopyQueryPoolResults) \
    \
    x(vkCreateFence) \
    x(vkDestroyFence) \
    x(vkResetFences) \
    x(vkGetFenceStatus) \
    x(vkWaitForFences) \
    \
    x(vkCreateSemaphore) \
    x(vkDestroySemaphore) \
    \
    x(vkCreateEvent) \
    x(vkDestroyEvent) \
    x(vkGetEventStatus) \
    x(vkSetEvent) \
    x(vkResetEvent) \
    \
    x(vkFreeCommandBuffers) \
    x(vkAllocateCommandBuffers) \
    x(vkBeginCommandBuffer) \
    x(vkEndCommandBuffer) \
    x(vkResetCommandBuffer) \
    x(vkResetCommandPool) \
    \
    x(vkBindImageMemory) \
    x(vkBindBufferMemory) \
    \
    x(vkCreateQueryPool) \
    x(vkGetQueryPoolResults) \
    x(vkDestroyQueryPool) \
    /* */

#if SGL_WINDOWS
#   define VULKAN_API_INSTANCE_PLATFORM_KHR_PROCS(x)          \
    x(vkCreateWin32SurfaceKHR) \
    /* */
#elif SGL_MACOS
#   define VULKAN_API_INSTANCE_PLATFORM_KHR_PROCS(x)          \
    x(vkCreateMetalSurfaceEXT) \
    /* */
#elif SGL_LINUX
#   define VULKAN_API_INSTANCE_PLATFORM_KHR_PROCS(x)          \
    x(vkCreateXlibSurfaceKHR) \
    /* */
#else
#   define VULKAN_API_INSTANCE_PLATFORM_KHR_PROCS(x)          \
    /* */
#endif

#define VULKAN_API_INSTANCE_KHR_PROCS(x)          \
    VULKAN_API_INSTANCE_PLATFORM_KHR_PROCS(x) \
    x(vkGetPhysicalDeviceSurfaceSupportKHR) \
    x(vkGetPhysicalDeviceSurfaceFormatsKHR) \
    x(vkGetPhysicalDeviceSurfacePresentModesKHR) \
    x(vkGetPhysicalDeviceSurfaceCapabilitiesKHR) \
    x(vkDestroySurfaceKHR) \

    /* */

#define VULKAN_API_DEVICE_KHR_PROCS(x) \
    x(vkQueuePresentKHR) \
    x(vkCreateSwapchainKHR) \
    x(vkGetSwapchainImagesKHR) \
    x(vkDestroySwapchainKHR) \
    x(vkAcquireNextImageKHR) \
    x(vkCreateRayTracingPipelinesKHR) \
    x(vkCmdTraceRaysKHR) \
    x(vkGetRayTracingShaderGroupHandlesKHR) \
    /* */

#if SGL_WINDOWS
#   define VULKAN_API_DEVICE_PLATFORM_OPT_PROCS(x) \
    x(vkGetMemoryWin32HandleKHR) \
    x(vkGetSemaphoreWin32HandleKHR) \
    /* */
#else
#   define VULKAN_API_DEVICE_PLATFORM_OPT_PROCS(x) \
    x(vkGetMemoryFdKHR) \
    x(vkGetSemaphoreFdKHR) \
    /* */
#endif

#define VULKAN_API_DEVICE_OPT_PROCS(x) \
    VULKAN_API_DEVICE_PLATFORM_OPT_PROCS(x) \
    x(vkCmdSetPrimitiveTopologyEXT) \
    x(vkGetBufferDeviceAddress) \
    x(vkGetBufferDeviceAddressKHR) \
    x(vkGetBufferDeviceAddressEXT) \
    x(vkCmdBuildAccelerationStructuresKHR) \
    x(vkCmdCopyAccelerationStructureKHR) \
    x(vkCmdCopyAccelerationStructureToMemoryKHR) \
    x(vkCmdCopyMemoryToAccelerationStructureKHR) \
    x(vkCmdWriteAccelerationStructuresPropertiesKHR) \
    x(vkCreateAccelerationStructureKHR) \
    x(vkDestroyAccelerationStructureKHR) \
    x(vkGetAccelerationStructureBuildSizesKHR) \
    x(vkGetSemaphoreCounterValue) \
    x(vkGetSemaphoreCounterValueKHR) \
    x(vkSignalSemaphore) \
    x(vkSignalSemaphoreKHR) \
    x(vkWaitSemaphores) \
    x(vkWaitSemaphoresKHR) \
    x(vkCmdSetSampleLocationsEXT) \
    x(vkCmdDebugMarkerBeginEXT) \
    x(vkCmdDebugMarkerEndEXT) \
    x(vkDebugMarkerSetObjectNameEXT) \
    x(vkCmdDrawMeshTasksEXT) \
    /* */

#define VULKAN_API_ALL_GLOBAL_PROCS(x) \
    VULKAN_API_GLOBAL_PROCS(x)

#define VULKAN_API_ALL_INSTANCE_PROCS(x) \
    VULKAN_API_INSTANCE_PROCS(x) \
    VULKAN_API_INSTANCE_KHR_PROCS(x)

#define VULKAN_API_ALL_DEVICE_PROCS(x) \
    VULKAN_API_DEVICE_PROCS(x) \
    VULKAN_API_DEVICE_KHR_PROCS(x) \
    VULKAN_API_DEVICE_OPT_PROCS(x)

#define VULKAN_API_ALL_PROCS(x) \
    VULKAN_API_ALL_GLOBAL_PROCS(x) \
    VULKAN_API_ALL_INSTANCE_PROCS(x) \
    VULKAN_API_ALL_DEVICE_PROCS(x) \
    \
    VULKAN_API_INSTANCE_PROCS_OPT(x) \
    /* */

// clang-format on

#define VULKAN_API_DECLARE_PROC(NAME) PFN_##NAME NAME = nullptr;

// ----------------------------------------------------------------------------
// VulkanApi
// ----------------------------------------------------------------------------

struct VulkanApi {
    VULKAN_API_ALL_PROCS(VULKAN_API_DECLARE_PROC)

    enum class ProcType {
        global,
        instance,
        device,
    };

    /// Returns true if all the functions in the category are defined.
    bool is_defined(ProcType type) const;

    /// Initialize global functions.
    bool init_global_procs(const VulkanModule& module);
    /// Initialize instance functions.
    bool init_instance_procs(VkInstance instance);
    /// Initialize the device functions.
    bool init_device_procs(VkDevice device);
};

} // namespace sgl::rhi::vulkan
