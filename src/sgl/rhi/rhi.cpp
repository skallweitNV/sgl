#include "rhi.h"

#include "sgl/core/format.h"
#include "sgl/core/string.h"
#include "sgl/math/vector.h"

namespace sgl::rhi {

std::string Viewport::to_string() const
{
    return fmt::format(
        "Viewport(min_x={}, max_x={}, min_y={}, max_y={}, min_z={}, max_z={})",
        min_x,
        max_x,
        min_y,
        max_y,
        min_z,
        max_z
    );
}

std::string Rect::to_string() const
{
    return fmt::format("Rect(min_x={}, max_x={}, min_y={}, max_y={})", min_x, max_x, min_y, max_y);
}

inline GraphicsAPI resolve_default_api(GraphicsAPI api)
{
    if (api == GraphicsAPI::automatic) {
#if SGL_WINDOWS
        return GraphicsAPI::d3d12;
#elif SGL_LINUX
        return GraphicsAPI::vulkan;
#elif SGL_MACOS
        return GraphicsAPI::metal;
#endif
    }
    return api;
}

namespace vulkan {
    extern std::vector<ref<Adapter>> enum_adapters();
}

std::vector<ref<Adapter>> enum_adapters(GraphicsAPI api)
{
    api = resolve_default_api(api);
    switch (api) {
    // case GraphicsAPI::d3d12:
    //     return d3d12::enum_adapters();
    case GraphicsAPI::vulkan:
        return vulkan::enum_adapters();
    // case GraphicsAPI::metal:
    //     return metal::enum_adapters();
    default:
        return {};
    }
}

ref<Adapter> default_adapter(GraphicsAPI api)
{
    std::vector<ref<Adapter>> adapters = enum_adapters(api);
    return adapters.empty() ? nullptr : adapters.front();
}

ref<Device> create_device(const DeviceDesc& desc, Adapter* adapter)
{
    if (!adapter)
        adapter = default_adapter();
    if (!adapter)
        return nullptr;
    return adapter->create_device(desc);
}

std::string DeviceLimits::to_string() const
{
    return fmt::format(
        "DeviceLimits(\n"
        "  max_texture_dimension_1d = {},\n"
        "  max_texture_dimension_2d = {},\n"
        "  max_texture_dimension_3d = {},\n"
        "  max_texture_dimension_cube = {},\n"
        "  max_texture_array_layers = {},\n"
        "  max_vertex_input_elements = {},\n"
        "  max_vertex_input_element_offset = {},\n"
        "  max_vertex_streams = {},\n"
        "  max_vertex_stream_stride = {},\n"
        "  max_compute_threads_per_group = {},\n"
        "  max_compute_thread_group_size = {},\n"
        "  max_compute_dispatch_thread_groups = {},\n"
        "  max_viewports = {},\n"
        "  max_viewport_dimensions = {},\n"
        "  max_framebuffer_dimensions = {},\n"
        "  max_shader_visible_samplers = {},\n"
        ")",
        max_texture_dimension_1d,
        max_texture_dimension_2d,
        max_texture_dimension_3d,
        max_texture_dimension_cube,
        max_texture_array_layers,
        max_vertex_input_elements,
        max_vertex_input_element_offset,
        max_vertex_streams,
        max_vertex_stream_stride,
        max_compute_threads_per_group,
        max_compute_thread_group_size,
        max_compute_dispatch_thread_groups,
        max_viewports,
        max_viewport_dimensions,
        max_framebuffer_dimensions,
        max_shader_visible_samplers
    );
}

std::string DeviceInfo::to_string() const
{
    return fmt::format(
        "DeviceInfo(\n"
        "  api = {},\n"
        "  limits = {},\n"
        "  features = {},\n"
        "  extended_features = {},\n"
        "  adapter_name = \"{}\",\n"
        "  timestamp_frequency = {},\n"
        ")",
        api,
        string::indent(limits.to_string()),
        features,
        extended_features,
        adapter_name,
        timestamp_frequency
    );
}

std::string AdapterInfo::to_string() const
{
    return fmt::format(
        "AdapterInfo(\n"
        "  name = \"{}\",\n"
        "  api = {},\n"
        "  vendor_id = 0x{:x},\n"
        "  device_id = 0x{:x},\n"
        "  luid = {},\n"
        ")",
        name,
        api,
        vendor_id,
        device_id,
        luid
    );
}


} // namespace sgl::rhi
