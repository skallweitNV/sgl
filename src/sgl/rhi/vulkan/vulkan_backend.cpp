#include "vulkan_backend.h"
#include "vulkan_utils.h"

#include "sgl/rhi/validation/validation.h"

#include "sgl/core/type_utils.h"

// #include "sgl/rhi/validation/validation.h"

namespace sgl::rhi::vulkan {

ref<Device> AdapterImpl::create_device(const DeviceDesc& desc)
{
    ref<Device> device(new DeviceImpl(desc, this));
    if (desc.enable_validation)
        device = validation::wrap_device(device);
    return device;
}

std::vector<ref<Adapter>> enum_adapters()
{
    std::vector<ref<Adapter>> adapters;

    VulkanModule module;
    if (!module.load())
        SGL_THROW("Failed to load Vulkan module");
    VulkanApi api;
    if (!api.init_global_procs(module))
        SGL_THROW("Failed to load Vulkan global procs");

    VkInstance instance;
    {
        VkInstanceCreateInfo create_info = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
        const char* enabled_extension_names[]
            = { VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
#if SGL_MACOS
                VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,
#endif
              };
#if SGL_MACOS
        create_info.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif
        create_info.enabledExtensionCount = narrow_cast<uint32_t>(std::size(enabled_extension_names));
        create_info.ppEnabledExtensionNames = &enabled_extension_names[0];
        SGL_VK_CHECK(api.vkCreateInstance(&create_info, nullptr, &instance));
    }

    // This will fail due to not loading any extensions.
    api.init_instance_procs(instance);

    // Make sure required functions for enumerating physical devices were loaded.
    if (api.vkEnumeratePhysicalDevices || api.vkGetPhysicalDeviceProperties) {
        uint32_t physical_device_count = 0;
        SGL_VK_CHECK(api.vkEnumeratePhysicalDevices(instance, &physical_device_count, nullptr));

        std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
        SGL_VK_CHECK(api.vkEnumeratePhysicalDevices(instance, &physical_device_count, physical_devices.data()));

        for (const auto& physical_device : physical_devices) {
            ref<AdapterImpl> adapter(new AdapterImpl());
            AdapterInfo& info = adapter->m_info;

            VkPhysicalDeviceProperties props;
            api.vkGetPhysicalDeviceProperties(physical_device, &props);
            info.name = props.deviceName;
            info.api = GraphicsAPI::vulkan;
            info.vendor_id = props.vendorID;
            info.device_id = props.deviceID;
            info.luid = utils::get_adapter_luid(api, physical_device);

            adapters.push_back(adapter);
        }
    }

    api.vkDestroyInstance(instance, nullptr);
    module.unload();

    return adapters;
}

} // namespace sgl::rhi::vulkan
