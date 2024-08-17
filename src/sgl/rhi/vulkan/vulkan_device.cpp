#include "vulkan_backend.h"
#include "vulkan_utils.h"

#include "sgl/core/type_utils.h"

#include <set>

namespace sgl::rhi::vulkan {

static VkBool32 debug_message_callback(
    VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT objectType,
    uint64_t object,
    size_t location,
    int32_t messageCode,
    const char* pLayerPrefix,
    const char* pMessage,
    void* pUserData
)
{
    SGL_UNUSED(flags, objectType, object, location, messageCode, pLayerPrefix, pMessage, pUserData);
    // return ((DeviceImpl*)pUserData)
    //     ->handleDebugMessage(flags, objType, srcObject, location, msgCode, pLayerPrefix, pMsg);
    return false;
}

DeviceImpl::DeviceImpl(const DeviceDesc& desc, AdapterImpl* adapter)
    : DeviceBase(desc)
    , m_allocator(m_ctx)
{
    m_info.api = GraphicsAPI::vulkan;
    m_info.limits = {};
    m_info.features = DeviceFeatures(0);
    m_info.extended_features = {};

    if (!m_ctx.module.load())
        SGL_THROW("Failed to load Vulkan module");
    if (!m_ctx.api.init_global_procs(m_ctx.module))
        SGL_THROW("Failed to load Vulkan global procs");

    initialize_instance_and_device(VK_NULL_HANDLE, VK_NULL_HANDLE, VK_NULL_HANDLE, adapter, desc.enable_api_validation);
}

// Result Device::init(const DeviceDesc& desc, IAdapter* adapter_)
// {
//     SLANG_RETURN_ON_FAIL(Base::init(desc));

//     // Adapter* adapter = checked_cast<Adapter*>(adapter_);

//     // if (!adapter) createVulkan

//     const DeviceDescVulkan* descVulkan = findStructInChain<DeviceDescVulkan>(desc.next);
//     if (descVulkan) {
//         printf("Found vulkan desc!\n");
//     }


//     return SLANG_E_NOT_IMPLEMENTED;
// }

DeviceImpl::~DeviceImpl()
{
    if (m_vk_debug_report_callback)
        m_ctx.api.vkDestroyDebugReportCallbackEXT(m_ctx.vk_instance, m_vk_debug_report_callback, nullptr);

    m_ctx.api.vkDestroyDevice(m_ctx.vk_device, m_ctx.vk_allocation_callbacks);
    m_ctx.api.vkDestroyInstance(m_ctx.vk_instance, m_ctx.vk_allocation_callbacks);

    m_ctx.module.unload();
}

void DeviceImpl::initialize_instance_and_device(
    VkInstance instance,
    VkPhysicalDevice physical_device,
    VkDevice device,
    AdapterImpl* adapter,
    bool enable_validation_layer
)
{
    // Create instance.
    if (!instance) {
        VkApplicationInfo application_info = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
        application_info.pApplicationName = "sgl";
        application_info.pEngineName = "sgl";
        application_info.apiVersion = VK_API_VERSION_1_1;
        application_info.engineVersion = 1;
        application_info.applicationVersion = 1;

        // Setup list of instance extensions.
        std::vector<const char*> instance_extensions;
#if SGL_MACOS
        instance_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#endif
        instance_extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
        instance_extensions.push_back(VK_KHR_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME);
        instance_extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#if SGL_WINDOWS
        instance_extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif SGL_LINUX
        instance_extensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
#elif SGL_MACOS
        instance_extensions.push_back(VK_EXT_METAL_SURFACE_EXTENSION_NAME);
#endif
        if (enable_validation_layer)
            instance_extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

        VkInstanceCreateInfo instance_create_info = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
#if SGL_MACOS
        instance_create_info.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif
        instance_create_info.pApplicationInfo = &application_info;
        instance_create_info.enabledExtensionCount = narrow_cast<uint32_t>(instance_extensions.size());
        instance_create_info.ppEnabledExtensionNames = instance_extensions.data();

        // Setup validation layers.
        std::vector<const char*> layer_names;
        VkValidationFeaturesEXT validation_features = {VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT};
        VkValidationFeatureEnableEXT validation_feature_enable[1] = {VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT};
        if (enable_validation_layer) {
            static const std::string VK_LAYER_KHRONOS_validation("VK_LAYER_KHRONOS_validation");

            uint32_t layer_properties_count;
            SGL_VK_CHECK(m_ctx.api.vkEnumerateInstanceLayerProperties(&layer_properties_count, nullptr));
            std::vector<VkLayerProperties> layer_properties(layer_properties_count);
            SGL_VK_CHECK(m_ctx.api.vkEnumerateInstanceLayerProperties(&layer_properties_count, layer_properties.data())
            );

            for (const auto& layer : layer_properties) {
                if (layer.layerName == VK_LAYER_KHRONOS_validation) {
                    layer_names.push_back(VK_LAYER_KHRONOS_validation.c_str());
                    break;
                }
            }

            if (!layer_names.empty()) {
                instance_create_info.enabledLayerCount = narrow_cast<uint32_t>(layer_names.size());
                instance_create_info.ppEnabledLayerNames = layer_names.data();

                // Include support for printf
                validation_features.enabledValidationFeatureCount
                    = narrow_cast<uint32_t>(std::size(validation_feature_enable));
                validation_features.pEnabledValidationFeatures = validation_feature_enable;
                instance_create_info.pNext = &validation_features;
            }
        }
        static const uint32_t API_VERSIONS[] = {
            VK_API_VERSION_1_3,
            VK_API_VERSION_1_2,
            VK_API_VERSION_1_1,
            VK_API_VERSION_1_0,
        };
        for (auto api_version : API_VERSIONS) {
            application_info.apiVersion = api_version;
            VkResult result = m_ctx.api.vkCreateInstance(&instance_create_info, nullptr, &instance);
            if (result == VK_SUCCESS)
                break;
        }
    }

    if (!instance)
        SGL_THROW("Failed to create Vulkan instance");
    if (!m_ctx.api.init_instance_procs(instance))
        SGL_THROW("Failed to load Vulkan instance procs");

    // Setup validation report callback.
    if (enable_validation_layer && m_ctx.api.vkCreateDebugReportCallbackEXT) {
        VkDebugReportCallbackCreateInfoEXT create_info = {VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT};
        create_info.pfnCallback = &debug_message_callback;
        create_info.pUserData = this;
        create_info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;

        SGL_VK_CHECK(
            m_ctx.api.vkCreateDebugReportCallbackEXT(instance, &create_info, nullptr, &m_vk_debug_report_callback)
        );
    }

    // Create physical device.
    if (!physical_device) {
        uint32_t physical_device_count = 0;
        SGL_VK_CHECK(m_ctx.api.vkEnumeratePhysicalDevices(instance, &physical_device_count, nullptr));

        std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
        SGL_VK_CHECK(m_ctx.api.vkEnumeratePhysicalDevices(instance, &physical_device_count, physical_devices.data()));

        // Use first physical device by default.
        int selected = 0;

        // Search for requested adapter.
        if (adapter) {
            selected = -1;
            for (int i = 0; i < physical_devices.size(); ++i) {
                if (utils::get_adapter_luid(m_ctx.api, physical_devices[i]) == adapter->info().luid) {
                    selected = i;
                    break;
                }
            }
        }

        if (selected >= 0 && selected < physical_devices.size())
            physical_device = physical_devices[selected];
    }

    if (!physical_device)
        SGL_THROW("Failed to find Vulkan physical device");

    m_ctx.api.vkGetPhysicalDeviceProperties(physical_device, &m_ctx.physical_device_properties);
    m_ctx.api.vkGetPhysicalDeviceFeatures(physical_device, &m_ctx.physical_device_features);

    // Fetch device properties.
    {
        const auto& props = m_ctx.physical_device_properties;

        m_info.adapter_name = props.deviceName;
        m_info.timestamp_frequency = uint64_t(1e9 / props.limits.timestampPeriod);

        DeviceLimits& limits = m_info.limits;
        limits = {};
        limits.max_texture_dimension_1d = props.limits.maxImageDimension1D;
        limits.max_texture_dimension_2d = props.limits.maxImageDimension2D;
        limits.max_texture_dimension_3d = props.limits.maxImageDimension3D;
        limits.max_texture_dimension_cube = props.limits.maxImageDimensionCube;
        limits.max_texture_array_layers = props.limits.maxImageArrayLayers;

        limits.max_vertex_input_elements = props.limits.maxVertexInputAttributes;
        limits.max_vertex_input_element_offset = props.limits.maxVertexInputAttributeOffset;
        limits.max_vertex_streams = props.limits.maxVertexInputBindings;
        limits.max_vertex_stream_stride = props.limits.maxVertexInputBindingStride;

        limits.max_compute_threads_per_group = props.limits.maxComputeWorkGroupInvocations;
        limits.max_compute_thread_group_size[0] = props.limits.maxComputeWorkGroupSize[0];
        limits.max_compute_thread_group_size[1] = props.limits.maxComputeWorkGroupSize[1];
        limits.max_compute_thread_group_size[2] = props.limits.maxComputeWorkGroupSize[2];
        limits.max_compute_dispatch_thread_groups[0] = props.limits.maxComputeWorkGroupCount[0];
        limits.max_compute_dispatch_thread_groups[1] = props.limits.maxComputeWorkGroupCount[1];
        limits.max_compute_dispatch_thread_groups[2] = props.limits.maxComputeWorkGroupCount[2];

        limits.max_viewports = props.limits.maxViewports;
        limits.max_viewport_dimensions[0] = props.limits.maxViewportDimensions[0];
        limits.max_viewport_dimensions[1] = props.limits.maxViewportDimensions[1];
        limits.max_framebuffer_dimensions[0] = props.limits.maxFramebufferWidth;
        limits.max_framebuffer_dimensions[1] = props.limits.maxFramebufferHeight;
        limits.max_framebuffer_dimensions[2] = props.limits.maxFramebufferLayers;

        limits.max_shader_visible_samplers = props.limits.maxPerStageDescriptorSamplers;
    }

    // Query the available extensions.
    uint32_t extension_count = 0;
    SGL_VK_CHECK(m_ctx.api.vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, nullptr));
    std::vector<VkExtensionProperties> extensions(extension_count);
    SGL_VK_CHECK(
        m_ctx.api.vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, extensions.data())
    );

    // Create set of available extension names.
    std::set<std::string> extension_names;
    for (const auto& extension : extensions)
        extension_names.insert(extension.extensionName);

    std::vector<const char*> device_extensions;
    device_extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    device_extensions.push_back(VK_KHR_SHADER_NON_SEMANTIC_INFO_EXTENSION_NAME);
#if SGL_MACOS
    device_extensions.push_back("VK_KHR_portability_subset");
#endif

    if (!device) {
        VkDeviceCreateInfo device_create_info = {VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
        device_create_info.queueCreateInfoCount = 1;
        device_create_info.pEnabledFeatures = &m_ctx.physical_device_features;

        float queue_priority = 0.f;
        VkDeviceQueueCreateInfo queue_create_info = {VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
        // queue_create_info.queueFamilyIndex = m_queueFamilyIndex;
        queue_create_info.queueFamilyIndex = 0;
        queue_create_info.queueCount = 1;
        queue_create_info.pQueuePriorities = &queue_priority;
        device_create_info.pQueueCreateInfos = &queue_create_info;

        // device_create_info.enabledExtensionCount = uint32_t(device_extensions.getCount());
        // device_create_info.ppEnabledExtensionNames = deviceExtensions.getBuffer();

        SGL_VK_CHECK(
            m_ctx.api.vkCreateDevice(physical_device, &device_create_info, m_ctx.vk_allocation_callbacks, &device)
        );
    }

    if (!device)
        SGL_THROW("Failed to create Vulkan device");
    if (!m_ctx.api.init_device_procs(device))
        SGL_THROW("Failed to load Vulkan device procs");

    m_ctx.vk_instance = instance;
    m_ctx.vk_physical_device = physical_device;
    m_ctx.vk_device = device;
}

} // namespace sgl::rhi::vulkan
