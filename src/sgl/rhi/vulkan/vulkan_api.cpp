#include "vulkan_api.h"

#include "sgl/core/error.h"

namespace sgl::rhi::vulkan {

// ----------------------------------------------------------------------------
// VulkanModule
// ----------------------------------------------------------------------------

bool VulkanModule::load()
{
    if (m_module)
        unload();

#if SGL_WINDOWS
    const char* library_name = "vulkan-1.dll";
#elif SGL_LINUX
    const char* library_name = "libvulkan.so.1";
#elif SGL_MACOS
    const char* library_name = "libvulkan.1.dylib";
#endif

    m_module = platform::load_shared_library(library_name);
    return m_module != nullptr;
}

void VulkanModule::unload()
{
    if (!m_module)
        return;

    platform::release_shared_library(m_module);
    m_module = nullptr;
}

PFN_vkVoidFunction VulkanModule::get_function(const char* name) const
{
    if (!m_module)
        return nullptr;

    return (PFN_vkVoidFunction)platform::get_proc_address(m_module, name);
}

// ----------------------------------------------------------------------------
// VulkanApi
// ----------------------------------------------------------------------------

#define VULKAN_API_CHECK_FUNCTION(x) &&(x != nullptr)
#define VULKAN_API_CHECK_FUNCTIONS(FUNCTION_LIST) true FUNCTION_LIST(VULKAN_API_CHECK_FUNCTION)

bool VulkanApi::is_defined(ProcType type) const
{
    switch (type) {
    case ProcType::global:
        return VULKAN_API_CHECK_FUNCTIONS(VULKAN_API_ALL_GLOBAL_PROCS);
    case ProcType::instance:
        return VULKAN_API_CHECK_FUNCTIONS(VULKAN_API_ALL_INSTANCE_PROCS);
    case ProcType::device:
        return VULKAN_API_CHECK_FUNCTIONS(VULKAN_API_DEVICE_PROCS);
    }
    return false;
}

bool VulkanApi::init_global_procs(const VulkanModule& module)
{
#define VULKAN_API_GET_GLOBAL_PROC(x) x = (PFN_##x)module.get_function(#x);

    // Initialize all the global functions
    VULKAN_API_ALL_GLOBAL_PROCS(VULKAN_API_GET_GLOBAL_PROC)

    return is_defined(ProcType::global);
}

bool VulkanApi::init_instance_procs(VkInstance instance)
{
    SGL_ASSERT(instance && vkGetInstanceProcAddr != nullptr);

#define VULKAN_API_GET_INSTANCE_PROC(x) x = (PFN_##x)vkGetInstanceProcAddr(instance, #x);

    VULKAN_API_ALL_INSTANCE_PROCS(VULKAN_API_GET_INSTANCE_PROC)

    // Get optional
    VULKAN_API_INSTANCE_PROCS_OPT(VULKAN_API_GET_INSTANCE_PROC)

    return is_defined(ProcType::instance);
}

bool VulkanApi::init_device_procs(VkDevice device)
{
    SGL_ASSERT(device && vkGetInstanceProcAddr != nullptr);

#define VULKAN_API_GET_DEVICE_PROC(x) x = (PFN_##x)vkGetDeviceProcAddr(device, #x);

    VULKAN_API_ALL_DEVICE_PROCS(VULKAN_API_GET_DEVICE_PROC)

    if (!vkGetBufferDeviceAddressKHR && vkGetBufferDeviceAddressEXT)
        vkGetBufferDeviceAddressKHR = vkGetBufferDeviceAddressEXT;
    if (!vkGetBufferDeviceAddress && vkGetBufferDeviceAddressKHR)
        vkGetBufferDeviceAddress = vkGetBufferDeviceAddressKHR;
    if (!vkGetSemaphoreCounterValue && vkGetSemaphoreCounterValueKHR)
        vkGetSemaphoreCounterValue = vkGetSemaphoreCounterValueKHR;
    if (!vkSignalSemaphore && vkSignalSemaphoreKHR)
        vkSignalSemaphore = vkSignalSemaphoreKHR;

    return is_defined(ProcType::device);
}

} // namespace sgl::rhi::vulkan
