// SPDX-License-Identifier: Apache-2.0

#include "nanobind.h"

#include "sgl/rhi/rhi.h"

namespace sgl::rhi {

inline void bind_rhi(nb::module_& m)
{
    nb::sgl_enum<GraphicsAPI>(m, "GraphicsAPI", D_NA(GraphicsAPI));

    nb::class_<NativeHandle>(m, "NativeHandle", D_NA(NativeHandle)) //
        .def_ro("value", &NativeHandle::value, D_NA(NativeHandle, value));

    nb::class_<Viewport>(m, "Viewport", D_NA(Viewport))
        .def(nb::init<>(), D_NA(Viewport, Viewport))
        .def(
            nb::init<float, float, float, float, float, float>(),
            "min_x"_a,
            "max_x"_a,
            "min_y"_a,
            "max_y"_a,
            "min_z"_a,
            "max_z"_a,
            D_NA(Viewport, Viewport, 2)
        )
        .def(nb::init<float, float>(), "width"_a, "height"_a, D_NA(Viewport, Viewport, 3))
        .def_rw("min_x", &Viewport::min_x, D_NA(Viewport, min_x))
        .def_rw("max_x", &Viewport::max_x, D_NA(Viewport, max_x))
        .def_rw("min_y", &Viewport::min_y, D_NA(Viewport, min_y))
        .def_rw("max_y", &Viewport::max_y, D_NA(Viewport, max_y))
        .def_rw("min_z", &Viewport::min_z, D_NA(Viewport, min_z))
        .def_rw("max_z", &Viewport::max_z, D_NA(Viewport, max_z))
        .def_prop_ro("width", &Viewport::width, D_NA(Viewport, width))
        .def_prop_ro("height", &Viewport::height, D_NA(Viewport, height))
        .def_prop_ro("depth", &Viewport::depth, D_NA(Viewport, depth))
        .def(nb::self == nb::self)
        .def(nb::self != nb::self)
        .def("__repr__", &Viewport::to_string);

    nb::class_<Rect>(m, "Rect", D_NA(Rect))
        .def(nb::init<>(), D_NA(Rect, Rect))
        .def(nb::init<float, float, float, float>(), "min_x"_a, "max_x"_a, "min_y"_a, "max_y"_a, D_NA(Rect, Rect, 2))
        .def(nb::init<float, float>(), "width"_a, "height"_a, D_NA(Rect, Rect, 3))
        .def_rw("min_x", &Rect::min_x, D_NA(Rect, min_x))
        .def_rw("max_x", &Rect::max_x, D_NA(Rect, max_x))
        .def_rw("min_y", &Rect::min_y, D_NA(Rect, min_y))
        .def_rw("max_y", &Rect::max_y, D_NA(Rect, max_y))
        .def_prop_ro("width", &Rect::width, D_NA(Rect, width))
        .def_prop_ro("height", &Rect::height, D_NA(Rect, height))
        .def(nb::self == nb::self)
        .def(nb::self != nb::self)
        .def("__repr__", &Rect::to_string);


    // -------------------------------------------------------------------------
    // Resource formats
    // -------------------------------------------------------------------------

    nb::sgl_enum<Format>(m, "Format", D_NA(Format));
    nb::sgl_enum<FormatKind>(m, "FormatKind", D_NA(FormatKind));
    nb::class_<FormatInfo>(m, "FormatInfo", D_NA(FormatInfo))
        .def_ro("format", &FormatInfo::format, D_NA(FormatInfo, format))
        .def_ro("name", &FormatInfo::name, D_NA(FormatInfo, name))
        .def_ro("kind", &FormatInfo::kind, D_NA(FormatInfo, kind))
        .def_ro("bytes_per_block", &FormatInfo::bytes_per_block, D_NA(FormatInfo, bytes_per_block))
        .def_ro("block_size", &FormatInfo::block_size, D_NA(FormatInfo, block_size))
        // clang-format off
        .def_prop_ro("has_red", [](const FormatInfo& self) { return self.has_red; }, D_NA(FormatInfo, has_red))
        .def_prop_ro("has_green", [](const FormatInfo& self) { return self.has_green; }, D_NA(FormatInfo, has_green))
        .def_prop_ro("has_blue", [](const FormatInfo& self) { return self.has_blue; }, D_NA(FormatInfo, has_blue))
        .def_prop_ro("has_alpha", [](const FormatInfo& self) { return self.has_alpha; }, D_NA(FormatInfo, has_alpha))
        .def_prop_ro("has_depth", [](const FormatInfo& self) { return self.has_depth; }, D_NA(FormatInfo, has_depth))
        .def_prop_ro("has_stencil", [](const FormatInfo& self) { return self.has_stencil; }, D_NA(FormatInfo, has_stencil))
        .def_prop_ro("is_signed", [](const FormatInfo& self) { return self.is_signed; }, D_NA(FormatInfo, is_signed))
        .def_prop_ro("is_srgb", [](const FormatInfo& self) { return self.is_srgb; }, D_NA(FormatInfo, is_srgb))
        // clang-format on
        .def("__repr__", &FormatInfo::to_string)
        .def_static("get", &FormatInfo::get, "format"_a, D_NA(FormatInfo, get));

    // nb::sgl_enum_flags<FormatSupport>(m, "FormatSupport", D_NA(FormatSupport));

    // -------------------------------------------------------------------------
    // Resources
    // -------------------------------------------------------------------------

    nb::class_<Resource, Object>(m, "Resource")
        .def("get_native_handle", &Resource::get_native_handle, D_NA(Resource, get_native_handle));

    nb::sgl_enum<MemoryType>(m, "MemoryType", D_NA(MemoryType));
    nb::sgl_enum_flags<ResourceState>(m, "ResourceState", D_NA(ResourceState));

    // -------------------------------------------------------------------------
    // Buffer
    // -------------------------------------------------------------------------

    nb::class_<BufferRange>(m, "BufferRange", D_NA(BufferRange))
        .def(nb::init<uint64_t, uint64_t>(), "size"_a, "offset"_a = 0, D_NA(BufferRange, BufferRange))
        .def_rw("size", &BufferRange::size, D_NA(BufferRange, size))
        .def_rw("offset", &BufferRange::offset, D_NA(BufferRange, offset));

    // TODO
    nb::class_<BufferDesc>(m, "BufferDesc", D_NA(BufferDesc));

    nb::class_<Buffer, Resource>(m, "Buffer")
        .def_prop_ro("desc", &Buffer::desc, D_NA(Buffer, desc))
        .def_prop_ro("device_address", &Buffer::device_address, D_NA(Buffer, device_address));

    // -------------------------------------------------------------------------
    // Texture
    // -------------------------------------------------------------------------

    nb::class_<TextureSlice>(m, "TextureSlice", D_NA(TextureSlice));
    // .def_rw("mip_level", &TextureSlice::mip_level, D_NA(TextureSlice, mip_level))
    // .def_rw("array_layer", &TextureSlice::array_layer, D_NA(TextureSlice, array_layer))
    // .def_rw("depth_layer", &TextureSlice::depth_layer, D_NA(TextureSlice, depth_layer));

    // TODO
    nb::class_<TextureSubresourceSet>(m, "TextureSubresourceSet", D_NA(TextureSubresourceSet));

    // TODO
    nb::class_<TextureDesc>(m, "TextureDesc", D_NA(TextureDesc));

    nb::class_<Texture, Resource>(m, "Texture")
        .def_prop_ro("desc", &Texture::desc, D_NA(Texture, desc))
        .def_prop_ro("device_address", &Texture::device_address, D_NA(Texture, device_address));

    // -------------------------------------------------------------------------
    // Sampler
    // -------------------------------------------------------------------------

    nb::sgl_enum<ComparisonFunc>(m, "ComparisonFunc", D_NA(ComparisonFunc));
    nb::sgl_enum<TextureFilteringMode>(m, "TextureFilteringMode", D_NA(TextureFilteringMode));
    nb::sgl_enum<TextureAddressingMode>(m, "TextureAddressingMode", D_NA(TextureAddressingMode));
    nb::sgl_enum<TextureReductionOp>(m, "TextureReductionOp", D_NA(TextureReductionOp));

    nb::class_<SamplerDesc>(m, "SamplerDesc", D_NA(SamplerDesc))
        .def_rw("min_filter", &SamplerDesc::min_filter, D_NA(SamplerDesc, min_filter))
        .def_rw("mag_filter", &SamplerDesc::mag_filter, D_NA(SamplerDesc, mag_filter))
        .def_rw("mip_filter", &SamplerDesc::mip_filter, D_NA(SamplerDesc, mip_filter))
        .def_rw("reduction_op", &SamplerDesc::reduction_op, D_NA(SamplerDesc, reduction_op))
        .def_rw("address_u", &SamplerDesc::address_u, D_NA(SamplerDesc, address_u))
        .def_rw("address_v", &SamplerDesc::address_v, D_NA(SamplerDesc, address_v))
        .def_rw("address_w", &SamplerDesc::address_w, D_NA(SamplerDesc, address_w))
        .def_rw("mip_bias", &SamplerDesc::mip_bias, D_NA(SamplerDesc, mip_bias))
        .def_rw("max_anisotropy", &SamplerDesc::max_anisotropy, D_NA(SamplerDesc, max_anisotropy))
        .def_rw("comparison_func", &SamplerDesc::comparison_func, D_NA(SamplerDesc, comparison_func))
        .def_rw("border_color", &SamplerDesc::border_color, D_NA(SamplerDesc, border_color))
        .def_rw("mip_min", &SamplerDesc::mip_min, D_NA(SamplerDesc, mip_min))
        .def_rw("mip_max", &SamplerDesc::mip_max, D_NA(SamplerDesc, mip_max));

    nb::class_<Sampler, Resource>(m, "Sampler", D_NA(Sampler)).def_prop_ro("desc", &Sampler::desc, D_NA(Sampler, desc));

    // -------------------------------------------------------------------------
    // InputLayout
    // -------------------------------------------------------------------------

    // -------------------------------------------------------------------------
    // Framebuffer
    // -------------------------------------------------------------------------

    // -------------------------------------------------------------------------
    // ShaderProgram
    // -------------------------------------------------------------------------

    nb::class_<ShaderProgram, Resource>(m, "ShaderProgram", D_NA(ShaderProgram));

    // -------------------------------------------------------------------------
    // ShaderObject
    // -------------------------------------------------------------------------

    // -------------------------------------------------------------------------
    // ComputePipeline
    // -------------------------------------------------------------------------

    nb::class_<ComputePipelineDesc>(m, "ComputePipelineDesc", D_NA(ComputePipelineDesc))
        .def_rw("program", &ComputePipelineDesc::program, D_NA(ComputePipelineDesc, program));

    nb::class_<ComputePipeline, Resource>(m, "ComputePipeline", D_NA(ComputePipeline));

    // -------------------------------------------------------------------------
    // GraphicsPipeline
    // -------------------------------------------------------------------------

    nb::sgl_enum<PrimitiveType>(m, "PrimitiveType", D_NA(PrimitiveType));

    nb::class_<GraphicsPipeline, Resource>(m, "GraphicsPipeline", D_NA(GraphicsPipeline));

    // -------------------------------------------------------------------------
    // RaytracingPipeline
    // -------------------------------------------------------------------------

    nb::class_<RaytracingPipeline, Resource>(m, "RaytracingPipeline", D_NA(RaytracingPipeline));

    // -------------------------------------------------------------------------
    // Fence
    // -------------------------------------------------------------------------

    // -------------------------------------------------------------------------
    // CommandList
    // -------------------------------------------------------------------------

    // -------------------------------------------------------------------------
    // Device
    // -------------------------------------------------------------------------

    nb::class_<DeviceDesc>(m, "DeviceDesc", D_NA(DeviceDesc))
        .def_rw("enable_validation", &DeviceDesc::enable_validation, D_NA(DeviceDesc, enable_validation))
        .def_rw("enable_api_validation", &DeviceDesc::enable_api_validation, D_NA(DeviceDesc, enable_api_validation));

    nb::class_<DeviceLimits>(m, "DeviceLimits", D_NA(DeviceLimits))
        // clang-format off
        .def_ro("max_texture_dimension_1d", &DeviceLimits::max_texture_dimension_1d, D_NA(DeviceLimits, max_texture_dimension_1d))
        .def_ro("max_texture_dimension_2d", &DeviceLimits::max_texture_dimension_2d, D_NA(DeviceLimits, max_texture_dimension_2d))
        .def_ro("max_texture_dimension_3d", &DeviceLimits::max_texture_dimension_3d, D_NA(DeviceLimits, max_texture_dimension_3d))
        .def_ro("max_texture_dimension_cube", &DeviceLimits::max_texture_dimension_cube, D_NA(DeviceLimits, max_texture_dimension_cube))
        .def_ro("max_texture_array_layers", &DeviceLimits::max_texture_array_layers, D_NA(DeviceLimits, max_texture_array_layers))
        .def_ro("max_vertex_input_elements", &DeviceLimits::max_vertex_input_elements, D_NA(DeviceLimits, max_vertex_input_elements))
        .def_ro("max_vertex_input_element_offset", &DeviceLimits::max_vertex_input_element_offset, D_NA(DeviceLimits, max_vertex_input_element_offset))
        .def_ro("max_vertex_streams", &DeviceLimits::max_vertex_streams, D_NA(DeviceLimits, max_vertex_streams))
        .def_ro("max_vertex_stream_stride", &DeviceLimits::max_vertex_stream_stride, D_NA(DeviceLimits, max_vertex_stream_stride))
        .def_ro("max_compute_threads_per_group", &DeviceLimits::max_compute_threads_per_group, D_NA(DeviceLimits, max_compute_threads_per_group))
        .def_ro("max_compute_thread_group_size", &DeviceLimits::max_compute_thread_group_size, D_NA(DeviceLimits, max_compute_thread_group_size))
        .def_ro("max_compute_dispatch_thread_groups", &DeviceLimits::max_compute_dispatch_thread_groups, D_NA(DeviceLimits, max_compute_dispatch_thread_groups))
        .def_ro("max_viewports", &DeviceLimits::max_viewports, D_NA(DeviceLimits, max_viewports))
        .def_ro("max_viewport_dimensions", &DeviceLimits::max_viewport_dimensions, D_NA(DeviceLimits, max_viewport_dimensions))
        .def_ro("max_framebuffer_dimensions", &DeviceLimits::max_framebuffer_dimensions, D_NA(DeviceLimits, max_framebuffer_dimensions))
        .def_ro("max_shader_visible_samplers", &DeviceLimits::max_shader_visible_samplers, D_NA(DeviceLimits, max_shader_visible_samplers))
        // clang-format on
        .def("__repr__", &DeviceLimits::to_string);

    nb::class_<DeviceInfo>(m, "DeviceInfo", D_NA(DeviceInfo))
        .def_ro("api", &DeviceInfo::api, D_NA(DeviceInfo, api))
        .def_ro("limits", &DeviceInfo::limits, D_NA(DeviceInfo, limits))
        .def_ro("features", &DeviceInfo::features, D_NA(DeviceInfo, features))
        .def_ro("extended_features", &DeviceInfo::extended_features, D_NA(DeviceInfo, extended_features))
        .def_ro("adapter_name", &DeviceInfo::adapter_name, D_NA(DeviceInfo, adapter_name))
        .def_ro("timestamp_frequency", &DeviceInfo::timestamp_frequency, D_NA(DeviceInfo, timestamp_frequency))
        .def("__repr__", &DeviceInfo::to_string);

    nb::class_<Device, Object>(m, "Device", D_NA(Device))
        .def_prop_ro("desc", &Device::desc, D_NA(Device, desc))
        .def_prop_ro("info", &Device::info, D_NA(Device, info))
        .def(
            "create_sampler",
            [](Device& self,
               TextureFilteringMode min_filter,
               TextureFilteringMode mag_filter,
               TextureFilteringMode mip_filter,
               TextureReductionOp reduction_op,
               TextureAddressingMode address_u,
               TextureAddressingMode address_v,
               TextureAddressingMode address_w,
               float mip_bias,
               uint32_t max_anisotropy,
               ComparisonFunc comparison_func,
               float4 border_color,
               float mip_min,
               float mip_max)
            {
                return self.create_sampler({
                    .min_filter = min_filter,
                    .mag_filter = mag_filter,
                    .mip_filter = mip_filter,
                    .reduction_op = reduction_op,
                    .address_u = address_u,
                    .address_v = address_v,
                    .address_w = address_w,
                    .mip_bias = mip_bias,
                    .max_anisotropy = max_anisotropy,
                    .comparison_func = comparison_func,
                    .border_color = border_color,
                    .mip_min = mip_min,
                    .mip_max = mip_max,
                });
            },
            "min_filter"_a = SamplerDesc{}.min_filter,
            "mag_filter"_a = SamplerDesc{}.mag_filter,
            "mip_filter"_a = SamplerDesc{}.mip_filter,
            "reduction_op"_a = SamplerDesc{}.reduction_op,
            "address_u"_a = SamplerDesc{}.address_u,
            "address_v"_a = SamplerDesc{}.address_v,
            "address_w"_a = SamplerDesc{}.address_w,
            "mip_bias"_a = SamplerDesc{}.mip_bias,
            "max_anisotropy"_a = SamplerDesc{}.max_anisotropy,
            "comparison_func"_a = SamplerDesc{}.comparison_func,
            "border_color"_a = SamplerDesc{}.border_color,
            "mip_min"_a = SamplerDesc{}.mip_min,
            "mip_max"_a = SamplerDesc{}.mip_max,
            D_NA(Device, create_sampler)
        )
        .def("create_sampler", &Device::create_sampler, "desc"_a, D_NA(Device, create_sampler));
    // .def(
    //     "create_buffer",
    //     [](Device& self, uint64_t size) { return self.create_buffer({.size = size}); },
    //     "size"_a
    // )
    // .def("create_buffer", &Device::create_buffer, D_NA(Device, create_buffer));

    // -------------------------------------------------------------------------
    // Adapter
    // -------------------------------------------------------------------------

    nb::class_<AdapterInfo>(m, "AdapterInfo", D_NA(AdapterInfo))
        .def_ro("name", &AdapterInfo::name, D_NA(AdapterInfo, name))
        .def_ro("api", &AdapterInfo::api, D_NA(AdapterInfo, api))
        .def_ro("vendor_id", &AdapterInfo::vendor_id, D_NA(AdapterInfo, vendor_id))
        .def_ro("device_id", &AdapterInfo::device_id, D_NA(AdapterInfo, device_id))
        .def_ro("luid", &AdapterInfo::luid, D_NA(AdapterInfo, luid))
        .def("__repr__", &AdapterInfo::to_string);

    nb::class_<Adapter, Object>(m, "Adapter", D_NA(Adapter))
        .def_prop_ro("info", &Adapter::info, D_NA(Adapter, info))
        .def("create_device", &Adapter::create_device, D_NA(Adapter, create_device));

    m.def("enum_adapters", &enum_adapters, "api"_a = GraphicsAPI::automatic, D_NA(enum_adapters));
    m.def("default_adapter", &default_adapter, "api"_a = GraphicsAPI::automatic, D_NA(default_adapter));
    m.def(
        "create_device",
        [](bool enable_validation, bool enable_api_validation, Adapter* adapter)
        {
            return create_device(
                {
                    .enable_validation = enable_validation,
                    .enable_api_validation = enable_api_validation,
                },
                adapter
            );
        },
        "enable_validation"_a = true,
        "enable_api_validation"_a = false,
        "adapter"_a.none() = nb::none(),
        D_NA(create_device)
    );
    m.def("create_device", &create_device, "desc"_a, "adapter"_a.none() = nb::none(), D_NA(create_device));
}

} // namespace sgl::rhi

SGL_PY_EXPORT(rhi)
{
    nb::module_ rhi = m.attr("rhi");

    sgl::rhi::bind_rhi(rhi);
}
