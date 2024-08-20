#pragma once

#include "sgl/core/object.h"
#include "sgl/core/enum.h"
#include "sgl/math/vector_types.h"

#include <array>
#include <string>
#include <vector>

namespace sgl::rhi {

class Adapter;

using DeviceAddress = uintptr_t;

// -----------------------------------------------------------------------------
// Constants
// -----------------------------------------------------------------------------

static constexpr uint32_t MAX_RENDER_TARGET_COUNT = 8;

struct SGL_API Viewport {
    float min_x{0.f};
    float max_x{0.f};
    float min_y{0.f};
    float max_y{0.f};
    float min_z{0.f};
    float max_z{1.f};

    Viewport() = default;
    Viewport(float min_x, float max_x, float min_y, float max_y, float min_z, float max_z)
        : min_x(min_x)
        , max_x(max_x)
        , min_y(min_y)
        , max_y(max_y)
        , min_z(min_z)
        , max_z(max_z)
    {
    }
    Viewport(float width, float height)
        : min_x(0.f)
        , max_x(width)
        , min_y(0.f)
        , max_y(height)
        , min_z(0.f)
        , max_z(1.f)
    {
    }

    [[nodiscard]] float width() const { return max_x - min_x; }
    [[nodiscard]] float height() const { return max_y - min_y; }
    [[nodiscard]] float depth() const { return max_z - min_z; }

    constexpr bool operator==(const Viewport& rhs) const
    {
        return min_x == rhs.min_x && max_x == rhs.max_x && min_y == rhs.min_y && max_y == rhs.max_y
            && min_z == rhs.min_z && max_z == rhs.max_z;
    }

    constexpr bool operator!=(const Viewport& rhs) const { return !(*this == rhs); }

    std::string to_string() const;
};

struct SGL_API Rect {
    float min_x{0.f};
    float max_x{0.f};
    float min_y{0.f};
    float max_y{0.f};

    Rect() = default;
    Rect(float min_x, float max_x, float min_y, float max_y)
        : min_x(min_x)
        , max_x(max_x)
        , min_y(min_y)
        , max_y(max_y)
    {
    }
    Rect(float width, float height)
        : min_x(0.f)
        , max_x(width)
        , min_y(0.f)
        , max_y(height)
    {
    }

    [[nodiscard]] float width() const { return max_x - min_x; }
    [[nodiscard]] float height() const { return max_y - min_y; }

    constexpr bool operator==(const Rect& rhs) const
    {
        return min_x == rhs.min_x && max_x == rhs.max_x && min_y == rhs.min_y && max_y == rhs.max_y;
    }

    constexpr bool operator!=(const Rect& rhs) const { return !(*this == rhs); }

    std::string to_string() const;
};

struct ShaderOffset {
    uint32_t uniform_offset{0};
    uint32_t binding_range_index{0};
    uint32_t binding_array_index{0};

    ShaderOffset() = default;
    ShaderOffset(uint32_t uniform_offset_, uint32_t binding_range_index_, uint32_t binding_array_index_)
        : uniform_offset(uniform_offset_)
        , binding_range_index(binding_range_index_)
        , binding_array_index(binding_array_index_)
    {
    }

    constexpr bool operator==(const ShaderOffset& rhs) const
    {
        return uniform_offset == rhs.uniform_offset && binding_range_index == rhs.binding_range_index
            && binding_array_index == rhs.binding_array_index;
    }
    constexpr bool operator!=(const ShaderOffset& rhs) const { return !(*this == rhs); }
    constexpr bool operator<(const ShaderOffset& rhs) const
    {
        if (binding_range_index < rhs.binding_range_index)
            return true;
        if (binding_range_index > rhs.binding_range_index)
            return false;
        if (binding_array_index < rhs.binding_array_index)
            return true;
        if (binding_array_index > rhs.binding_array_index)
            return false;
        return uniform_offset < rhs.uniform_offset;
    }
    constexpr bool operator<=(const ShaderOffset& rhs) const { return (*this == rhs) || (*this < rhs); }
    constexpr bool operator>(const ShaderOffset& rhs) const { return (rhs < *this); }
    constexpr bool operator>=(const ShaderOffset& rhs) const { return (rhs <= *this); }
};

enum class GraphicsAPI {
    automatic,
    d3d12,
    vulkan,
    metal,
    cuda,
};

SGL_ENUM_INFO(
    GraphicsAPI,
    {
        {GraphicsAPI::automatic, "automatic"},
        {GraphicsAPI::d3d12, "d3d12"},
        {GraphicsAPI::vulkan, "vulkan"},
        {GraphicsAPI::metal, "metal"},
        {GraphicsAPI::cuda, "cuda"},
    }
);
SGL_ENUM_REGISTER(GraphicsAPI);

enum class NativeHandleType {
    win32_handle = 0x000,    ///< General Win32 HANDLE.
    file_descriptor = 0x001, ///< General file descriptor.
    shared_handle = 0x002,   ///< General shared handle.

    d3d12_device = 0x100,
    d3d12_command_queue = 0x101,
    d3d12_command_list = 0x102,
    d3d12_resource = 0x103,
    d3d12_render_target_view_descriptor = 0x104,
    d3d12_depth_stencil_view_descriptor = 0x105,
    d3d12_shader_resource_view_gpu_descripror = 0x106,
    d3d12_unordered_access_view_gpu_descripror = 0x107,
    d3d12_root_signature = 0x108,
    d3d12_pipeline_state = 0x109,
    d3d12_command_allocator = 0x10a,

    vk_device = 0x200,
    vk_physical_device = 0x201,
    vk_instance = 0x202,
    vk_queue = 0x203,
    vk_command_buffer = 0x204,
    vk_device_memory = 0x205,
    vk_buffer = 0x206,
    vk_image = 0x207,
    vk_image_view = 0x208,
    vk_acceleration_structure_khr = 0x209,
    vk_sampler = 0x20a,
    vk_shader_module = 0x20b,
    vk_render_pass = 0x20c,
    vk_framebuffer = 0x20d,
    vk_descriptor_pool = 0x20e,
    vk_descriptor_set_layout = 0x21f,
    vk_descriptor_set = 0x210,
    vk_pipeline_layout = 0x211,
    vk_pipeline = 0x212,
    vk_micromap = 0x213,

    mtl_device = 0x300,
    mtl_heap = 0x301,
    mtl_buffer = 0x302,
    mtl_texture = 0x303,
    mtl_sampler_state = 0x304,

    cu_device = 0x400,
    cu_device_ptr = 0x401,
};

struct NativeHandle {
    static constexpr uint64_t INVALID_VALUE = uint64_t(-1);

    uint64_t value{INVALID_VALUE};

    NativeHandle() = default;
    explicit NativeHandle(uint64_t value)
        : value(value)
    {
    }
    explicit NativeHandle(void* value)
        : value(reinterpret_cast<uintptr_t>(value))
    {
    }

    bool valid() const { return value != INVALID_VALUE; }

    template<typename T>
    T* as() const
    {
        return reinterpret_cast<T*>(value);
    }
};

// -----------------------------------------------------------------------------
// Resource formats
// -----------------------------------------------------------------------------

enum class Format : uint32_t {
    unknown,

    r8_uint,
    r8_sint,
    r8_unorm,
    r8_snorm,
    rg8_uint,
    rg8_sint,
    rg8_unorm,
    rg8_snorm,
    r16_uint,
    r16_sint,
    r16_unorm,
    r16_snorm,
    r16_float,
    bgra4_unorm,
    b5g6r5_unorm,
    b5g5r5a1_unorm,
    rgba8_uint,
    rgba8_sint,
    rgba8_unorm,
    rgba8_snorm,
    bgra8_unorm,
    srgba8_unorm,
    sbgra8_unorm,
    r10g10b10a2_unorm,
    r11g11b10_float,
    rg16_uint,
    rg16_sint,
    rg16_unorm,
    rg16_snorm,
    rg16_float,
    r32_uint,
    r32_sint,
    r32_float,
    rgba16_uint,
    rgba16_sint,
    rgba16_float,
    rgba16_unorm,
    rgba16_snorm,
    rg32_uint,
    rg32_sint,
    rg32_float,
    rgb32_uint,
    rgb32_sint,
    rgb32_float,
    rgba32_uint,
    rgba32_sint,
    rgba32_float,

    d16,
    d24s8,
    x24g8_uint,
    d32,
    d32s8,
    x32g8_uint,

    bc1_unorm,
    bc1_unorm_srgb,
    bc2_unorm,
    bc2_unorm_srgb,
    bc3_unorm,
    bc3_unorm_srgb,
    bc4_unorm,
    bc4_snorm,
    bc5_unorm,
    bc5_snorm,
    bc6h_ufloat,
    bc6h_sfloat,
    bc7_unorm,
    bc7_unorm_srgb,

    COUNT,
};

enum class FormatKind : uint8_t {
    integer,
    normalized,
    float_,
    depth_stencil,
};

SGL_ENUM_INFO(
    FormatKind,
    {
        {FormatKind::integer, "integer"},
        {FormatKind::normalized, "normalized"},
        {FormatKind::float_, "float_"},
        {FormatKind::depth_stencil, "depth_stencil"},
    }
);
SGL_ENUM_REGISTER(FormatKind);

struct SGL_API FormatInfo {
    Format format;
    const char* name;
    FormatKind kind;
    uint8_t bytes_per_block;
    uint8_t block_size;
    bool has_red : 1;
    bool has_green : 1;
    bool has_blue : 1;
    bool has_alpha : 1;
    bool has_depth : 1;
    bool has_stencil : 1;
    bool is_signed : 1;
    bool is_srgb : 1;

    std::string to_string() const;

    static const FormatInfo& get(Format format);
};

// Manually define the struct that SGL_ENUM_INFO(Format) would generate so we
// can use the existing table of resource formats.
struct Format_info {
    static const std::string& name()
    {
        static const std::string name = "Format";
        return name;
    }

    static std::span<std::pair<Format, std::string>> items()
    {
        auto create_items = []()
        {
            std::vector<std::pair<Format, std::string>> items((size_t)Format::COUNT);
            for (size_t i = 0; i < (size_t)Format::COUNT; ++i)
                items[i] = std::make_pair(Format(i), FormatInfo::get(Format(i)).name);
            return items;
        };
        static std::vector<std::pair<Format, std::string>> items = create_items();
        return items;
    }
};
SGL_ENUM_REGISTER(Format);

enum class FormatSupport : uint32_t {
    none = 0,

    buffer = (1 << 0),
    index_buffer = (1 << 1),
    vertex_buffer = (1 << 2),

    texture = (1 << 3),
    depth_stencil = (1 << 4),
    render_target = (1 << 5),
    blendable = (1 << 6),

    shader_load = (1 << 7),
    shader_sample = (1 << 8),
    shader_uav_load = (1 << 9),
    shader_uav_store = (1 << 10),
    shader_atomic = (1 << 11),
};

SGL_ENUM_INFO(
    FormatSupport,
    {
        {FormatSupport::none, "none"},
        {FormatSupport::buffer, "buffer"},
        {FormatSupport::index_buffer, "index_buffer"},
        {FormatSupport::vertex_buffer, "vertex_buffer"},
        {FormatSupport::texture, "texture"},
        {FormatSupport::depth_stencil, "depth_stencil"},
        {FormatSupport::render_target, "render_target"},
        {FormatSupport::blendable, "blendable"},
        {FormatSupport::shader_load, "shader_load"},
        {FormatSupport::shader_sample, "shader_sample"},
        {FormatSupport::shader_uav_load, "shader_uav_load"},
        {FormatSupport::shader_uav_store, "shader_uav_store"},
        {FormatSupport::shader_atomic, "shader_atomic"},
    }
);
SGL_ENUM_REGISTER(FormatSupport);
SGL_ENUM_CLASS_OPERATORS(FormatSupport);

// -----------------------------------------------------------------------------
// Resources
// -----------------------------------------------------------------------------

class Resource : public Object {
    SGL_OBJECT(Resource)
public:
    virtual NativeHandle get_native_handle(NativeHandleType type) const = 0;
};

enum class MemoryType : uint32_t {
    device_local,
    upload,
    read_back,
};

SGL_ENUM_INFO(
    MemoryType,
    {
        {MemoryType::device_local, "device_local"},
        {MemoryType::upload, "upload"},
        {MemoryType::read_back, "read_back"},
    }
);
SGL_ENUM_REGISTER(MemoryType);

struct SizeAndAlign {
    uint64_t size{0};
    uint64_t align{0};
};

enum class ResourceState : uint32_t {
    undefined = 0,
    common = (1 << 0),
    constant_buffer = (1 << 1),
    vertex_buffer = (1 << 2),
    index_buffer = (1 << 3),
    indirect_argument = (1 << 4),
    shader_resource = (1 << 5),
    unordered_access = (1 << 6),
    render_target = (1 << 7),
    depth_write = (1 << 8),
    depth_read = (1 << 9),
    stream_output = (1 << 10),
    copy_source = (1 << 11),
    copy_destination = (1 << 12),
    resolve_source = (1 << 13),
    resolve_destination = (1 << 14),
    present = (1 << 15),
    acceleration_structure = (1 << 16),
    acceleration_structure_build_input = (1 << 17),
};

SGL_ENUM_INFO(
    ResourceState,
    {
        {ResourceState::undefined, "undefined"},
        {ResourceState::common, "common"},
        {ResourceState::constant_buffer, "constant_buffer"},
        {ResourceState::vertex_buffer, "vertex_buffer"},
        {ResourceState::index_buffer, "index_buffer"},
        {ResourceState::indirect_argument, "indirect_argument"},
        {ResourceState::shader_resource, "shader_resource"},
        {ResourceState::unordered_access, "unordered_access"},
        {ResourceState::render_target, "render_target"},
        {ResourceState::depth_write, "depth_write"},
        {ResourceState::depth_read, "depth_read"},
        {ResourceState::stream_output, "stream_output"},
        {ResourceState::copy_source, "copy_source"},
        {ResourceState::copy_destination, "copy_destination"},
        {ResourceState::resolve_source, "resolve_source"},
        {ResourceState::resolve_destination, "resolve_destination"},
        {ResourceState::present, "present"},
        {ResourceState::acceleration_structure, "acceleration_structure"},
        {ResourceState::acceleration_structure_build_input, "acceleration_structure_build_input"},
    }
);
SGL_ENUM_REGISTER(ResourceState);
SGL_ENUM_CLASS_OPERATORS(ResourceState);

// -----------------------------------------------------------------------------
// Heap
// -----------------------------------------------------------------------------

struct HeapDesc {
    uint64_t size{0};
    MemoryType memory_type{MemoryType::device_local};
    std::string debug_name;
};

class Heap : public Resource {
    SGL_OBJECT(Heap)
public:
    virtual const HeapDesc& desc() const = 0;
};

// -----------------------------------------------------------------------------
// Buffer
// -----------------------------------------------------------------------------

struct BufferRange {
    uint64_t size{0};
    uint64_t offset{0};

    BufferRange() = default;
    BufferRange(uint64_t size_, uint64_t offset_)
        : size(size_)
        , offset(offset_)
    {
    }

    // [[nodiscard]] NVRHI_API BufferRange resolve(const BufferDesc& desc) const;
    // [[nodiscard]] constexpr bool isEntireBuffer(const BufferDesc& desc) const { return (offset == 0) && (size
    // == ~0ull || size == desc.size); }

    constexpr bool operator==(const BufferRange& rhs) const { return size == rhs.size && offset == rhs.offset; }
    constexpr bool operator!=(const BufferRange& rhs) const { return !(*this == rhs); }
};

static const BufferRange ENTIRE_BUFFER{0ull, ~0ull};

struct BufferDesc {
    /// Size of the buffer in bytes.
    uint64_t size{0};
    /// Struct stride for structured buffers.
    uint64_t struct_stride{0};
    /// Format used for typed buffer views.
    Format format{Format::unknown};

    /// Debug name for validation layers.
    std::string debug_name;

    MemoryType memory_type{MemoryType::device_local};
    ResourceState default_state{ResourceState::undefined};
    ResourceState allowed_states{ResourceState::undefined};

    bool is_vertex_buffer{false};
    bool is_index_buffer{false};

    bool is_shared{false};
};

class Buffer : public Resource {
    SGL_OBJECT(Buffer)
public:
    virtual const BufferDesc& desc() const = 0;
    virtual DeviceAddress device_address() const = 0;
};

// -----------------------------------------------------------------------------
// Texture
// -----------------------------------------------------------------------------

enum class TextureDimension : uint8_t {
    unknown,
    texture_1d,
    texture_1d_array,
    texture_2d,
    texture_2d_array,
    texture_cube,
    texture_cube_array,
    texture_2d_ms,
    texture_2d_ms_array,
    texture_3d,
};

SGL_ENUM_INFO(
    TextureDimension,
    {
        {TextureDimension::unknown, "unknown"},
        {TextureDimension::texture_1d, "texture_1d"},
        {TextureDimension::texture_1d_array, "texture_1d_array"},
        {TextureDimension::texture_2d, "texture_2d"},
        {TextureDimension::texture_2d_array, "texture_2d_array"},
        {TextureDimension::texture_cube, "texture_cube"},
        {TextureDimension::texture_cube_array, "texture_cube_array"},
        {TextureDimension::texture_2d_ms, "texture_2d_ms"},
        {TextureDimension::texture_2d_ms_array, "texture_2d_ms_array"},
        {TextureDimension::texture_3d, "texture_3d"},
    }
);
SGL_ENUM_REGISTER(TextureDimension);

using MipLevel = uint32_t;
using ArraySlice = uint32_t;

// describes a 2D section of a single mip level + single slice of a texture
struct TextureSlice {
    uint32_t x{0};
    uint32_t y{0};
    uint32_t z{0};
    // -1 means the entire dimension is part of the region
    // resolve() below will translate these values into actual dimensions
    uint32_t width{uint32_t(-1)};
    uint32_t height{uint32_t(-1)};
    uint32_t depth{uint32_t(-1)};

    MipLevel mipLevel{0};
    ArraySlice arraySlice{0};
};

struct TextureSubresourceSet { };

static const TextureSubresourceSet ENTIRE_TEXTURE{};

struct TextureDesc {
    TextureDimension type{TextureDimension::unknown};
    Format format{Format::unknown};
    uint32_t width{0};
    uint32_t height{0};
    uint32_t depth{0};
    uint32_t array_size{0};
    uint32_t mip_levels{0};
    uint32_t sample_count{1};
    uint32_t sample_quality{0};

    std::string debug_name;

    bool is_shader_resource{false};
    bool is_render_target{false};
    bool is_uav{false};
    bool is_typeless{false};
};

class Texture : public Resource {
    SGL_OBJECT(Resource)
public:
    virtual const TextureDesc& desc() const = 0;
    virtual DeviceAddress device_address() const = 0;
};

// -----------------------------------------------------------------------------
// Sampler
// -----------------------------------------------------------------------------

enum class ComparisonFunc : uint8_t {
    never,
    less,
    equal,
    less_equal,
    greater,
    not_equal,
    greater_equal,
    always,
};

SGL_ENUM_INFO(
    ComparisonFunc,
    {
        {ComparisonFunc::never, "never"},
        {ComparisonFunc::less, "less"},
        {ComparisonFunc::equal, "equal"},
        {ComparisonFunc::less_equal, "less_equal"},
        {ComparisonFunc::greater, "greater"},
        {ComparisonFunc::not_equal, "not_equal"},
        {ComparisonFunc::greater_equal, "greater_equal"},
        {ComparisonFunc::always, "always"},
    }
);
SGL_ENUM_REGISTER(ComparisonFunc);

enum class TextureFilteringMode : uint8_t {
    point,
    linear,
};

SGL_ENUM_INFO(
    TextureFilteringMode,
    {
        {TextureFilteringMode::point, "point"},
        {TextureFilteringMode::linear, "linear"},
    }
);
SGL_ENUM_REGISTER(TextureFilteringMode);

enum class TextureAddressingMode : uint8_t {
    wrap,
    clamp_to_edge,
    clamp_to_border,
    mirror_repeat,
    mirror_once,
};

SGL_ENUM_INFO(
    TextureAddressingMode,
    {
        {TextureAddressingMode::wrap, "wrap"},
        {TextureAddressingMode::clamp_to_edge, "clamp_to_edge"},
        {TextureAddressingMode::clamp_to_border, "clamp_to_border"},
        {TextureAddressingMode::mirror_repeat, "mirror_repeat"},
        {TextureAddressingMode::mirror_once, "mirror_once"},
    }
);
SGL_ENUM_REGISTER(TextureAddressingMode);

enum class TextureReductionOp : uint8_t {
    average,
    comparison,
    minimum,
    maximum,
};

SGL_ENUM_INFO(
    TextureReductionOp,
    {
        {TextureReductionOp::average, "average"},
        {TextureReductionOp::comparison, "comparison"},
        {TextureReductionOp::minimum, "minimum"},
        {TextureReductionOp::maximum, "maximum"},
    }
);
SGL_ENUM_REGISTER(TextureReductionOp);

struct SamplerDesc {
    TextureFilteringMode min_filter{TextureFilteringMode::linear};
    TextureFilteringMode mag_filter{TextureFilteringMode::linear};
    TextureFilteringMode mip_filter{TextureFilteringMode::linear};
    TextureReductionOp reduction_op{TextureReductionOp::average};
    TextureAddressingMode address_u{TextureAddressingMode::wrap};
    TextureAddressingMode address_v{TextureAddressingMode::wrap};
    TextureAddressingMode address_w{TextureAddressingMode::wrap};
    float mip_bias{0.f};
    uint32_t max_anisotropy{1};
    ComparisonFunc comparison_func{ComparisonFunc::never};
    float4 border_color{1.f};
    float mip_min{-1000.f};
    float mip_max{1000.f};
};

class Sampler : public Resource {
    SGL_OBJECT(Sampler)
public:
    virtual const SamplerDesc& desc() const = 0;
};

// -----------------------------------------------------------------------------
// InputLayout
// -----------------------------------------------------------------------------

struct InputAttributeDesc {
    /// Name of the attribute.
    std::string name;
    /// Format of the element.
    Format format{Format::unknown};
    uint32_t array_size{1};
    uint32_t buffer_index{0};
    uint32_t offset{0};
    /// Element stride in bytes.
    /// Note: For most graphics APIs, all strides for a given bufferIndex must be identical.
    uint32_t byte_stride{0};
    /// If true, this is a per-instance attribute.
    bool is_instanced{false};
};

struct InputLayoutDesc {
    std::vector<InputAttributeDesc> attributes;
};

class InputLayout : public Resource {
    SGL_OBJECT(InputLayout)
public:
};

// -----------------------------------------------------------------------------
// Framebuffer
// -----------------------------------------------------------------------------

struct FramebufferAttachmentDesc {
    ref<Texture> texture;
    // TextureSubresourceSet subresources = TextureSubresourceSet(0, 1, 0, 1);
    Format format{Format::unknown};
    bool is_read_only{false};
    // [[nodiscard]] bool valid() const { return texture != nullptr; }
};

struct FramebufferDesc {
    FramebufferAttachmentDesc color_attachments[MAX_RENDER_TARGET_COUNT];
    FramebufferAttachmentDesc depth_stencil_attachment;
};

struct FramebufferInfo {
    Format color_formats[MAX_RENDER_TARGET_COUNT] = {};
    Format depth_stencil_format{Format::unknown};
    uint32_t sample_count{1};
    uint32_t sample_quality{0};

    constexpr bool operator==(const FramebufferInfo& rhs) const
    {
        if (sample_count != rhs.sample_count || sample_quality != rhs.sample_quality)
            return false;
        for (int i = 0; i < MAX_RENDER_TARGET_COUNT; i++)
            if (color_formats[i] != rhs.color_formats[i])
                return false;
        return depth_stencil_format == rhs.depth_stencil_format;
    }
    constexpr bool operator!=(const FramebufferInfo& rhs) const { return !(*this == rhs); }
};

class Framebuffer : public Resource {
    SGL_OBJECT(Framebuffer)
public:
    virtual const FramebufferDesc& desc() const = 0;
    virtual const FramebufferInfo& info() const = 0;
};

// -----------------------------------------------------------------------------
// ShaderProgram
// -----------------------------------------------------------------------------

struct ShaderProgramDesc { };

class ShaderProgram : public Resource {
    SGL_OBJECT(ShaderProgram)
public:
};

// -----------------------------------------------------------------------------
// ShaderObject
// -----------------------------------------------------------------------------

class ShaderObject : public Resource {
    SGL_OBJECT(ShaderObject)
public:
    virtual void set_buffer(ShaderOffset offset, ref<Buffer> buffer, const BufferRange& range = ENTIRE_BUFFER) = 0;
    virtual void set_texture(
        ShaderOffset offset,
        ref<Texture> texture,
        const TextureSubresourceSet& subresource_set = ENTIRE_TEXTURE
    ) = 0;
    virtual void set_sampler(ShaderOffset offset, ref<Sampler> sampler) = 0;
};

class ImmutableShaderObject : public ShaderObject {
    SGL_OBJECT(ImmutableShaderObject)
public:
    virtual void finalize() = 0;
};

class MutableShaderObject : public ShaderObject {
    SGL_OBJECT(MutableShaderObject)
public:
};

// -----------------------------------------------------------------------------
// Pipeline
// -----------------------------------------------------------------------------

class Pipeline : public Resource {
    SGL_OBJECT(Pipeline)
public:
};

// -----------------------------------------------------------------------------
// ComputePipeline
// -----------------------------------------------------------------------------

struct ComputePipelineDesc {
    ref<ShaderProgram> program;
};

class ComputePipeline : public Pipeline {
    SGL_OBJECT(ComputePipeline)
public:
};

// -----------------------------------------------------------------------------
// GraphicsPipeline
// -----------------------------------------------------------------------------

enum class PrimitiveType {
    point,
    line,
    triangle,
    patch,
};

SGL_ENUM_INFO(
    PrimitiveType,
    {
        {PrimitiveType::point, "point"},
        {PrimitiveType::line, "line"},
        {PrimitiveType::triangle, "triangle"},
        {PrimitiveType::patch, "patch"},
    }
);
SGL_ENUM_REGISTER(PrimitiveType);

struct DepthStencilDesc {
    bool depth_test_enable{false};
    bool depth_write_enable{true};
    ComparisonFunc depth_func{ComparisonFunc::less};

    bool stencil_enable{false};
    uint32_t stencil_read_mask{0xFFFFFFFF};
    uint32_t stencil_write_mask{0xFFFFFFFF};
    // DepthStencilOpDesc frontFace;
    // DepthStencilOpDesc backFace;
    uint32_t stencil_ref{0};
};

struct RasterizerDesc {
    // FillMode fillMode = FillMode::Solid;
    // CullMode cullMode = CullMode::None;
    // FrontFaceMode frontFace = FrontFaceMode::CounterClockwise;
    int32_t depth_bias{0};
    float depth_bias_clamp{0.f};
    float slope_scaled_depth_bias{0.f};
    bool depth_clip_enable{true};
    bool scissor_enable{false};
    bool multisample_enable{false};
    bool antialiased_line_enable{false};
    bool enable_conservative_rasterization{false};
    uint32_t forced_sample_count{0};
};

struct GraphicsPipelineDesc {
    ref<ShaderProgram> program;
    ref<InputLayout> input_layout;
    ref<Framebuffer> framebuffer;
    // PrimitiveType primitive_type = PrimitiveType::TriangleList;
    DepthStencilDesc depth_stencil;
    RasterizerDesc rasterizer;
    // BlendDesc           blend;
};

class GraphicsPipeline : public Pipeline {
    SGL_OBJECT(GraphicsPipeline)
public:
};

// -----------------------------------------------------------------------------
// RaytracingPipeline
// -----------------------------------------------------------------------------

struct RaytracingPipelineDesc { };

class RaytracingPipeline : public Pipeline {
    SGL_OBJECT(RaytracingPipeline)
public:
};

// -----------------------------------------------------------------------------
// Fence
// -----------------------------------------------------------------------------

struct FenceDesc {
    uint64_t initial_value{0};
    bool is_shared{false};
};

class Fence : public Resource {
    SGL_OBJECT(Fence)
public:
    /// Returns the currently signaled value on the device.
    virtual uint64_t current_value() const = 0;
};

// -----------------------------------------------------------------------------
// CommandList
// -----------------------------------------------------------------------------

struct CommandListDesc { };

class CommandList : public Resource {
    SGL_OBJECT(CommandList)
public:
    virtual void open() = 0;
    virtual void close() = 0;

    virtual void clear_texture_float(ref<Texture> texture, const TextureSubresourceSet& subresources, float4 color) = 0;
    virtual void clear_texture_uint(ref<Texture> texture, const TextureSubresourceSet& subresources, uint32_t color)
        = 0;
    virtual void clear_depth_stencil_texture(
        ref<Texture> texture,
        const TextureSubresourceSet& subresources,
        bool clear_depth,
        float depth,
        bool clear_stencil,
        uint8_t stencil
    ) = 0;

    virtual void bind_compute_pipeline(ref<ComputePipeline> pipeline, ref<ShaderObject> root_object);
    virtual void dispatch(uint32_t x, uint32_t y, uint32_t z) = 0;
    virtual void dispatch_indirect(ref<Buffer> cmd_buffer, uint64_t offset);

    virtual void begin_marker(const char* name) = 0;
    virtual void end_marker() = 0;
};

// -----------------------------------------------------------------------------
// Device
// -----------------------------------------------------------------------------

enum class DeviceFeatures {
    border_color = (1 << 0),
    acceleration_structure = (1 << 1),
    raytracing_pipeline = (1 << 2),
    ray_query = (1 << 3),
};

SGL_ENUM_INFO(
    DeviceFeatures,
    {
        {DeviceFeatures::border_color, "border_color"},
        {DeviceFeatures::acceleration_structure, "acceleration_structure"},
        {DeviceFeatures::raytracing_pipeline, "raytracing_pipeline"},
        {DeviceFeatures::ray_query, "ray_query"},
    }
);
SGL_ENUM_REGISTER(DeviceFeatures);
SGL_ENUM_CLASS_OPERATORS(DeviceFeatures);

struct DeviceDesc {
    /// Enable RHI's validation layer.
    bool enable_validation{true};

    /// Enable the underlying API's validation layer (if available).
    bool enable_api_validation{false};

    // IMessageCallback* messageCallback;
};

struct SGL_API DeviceLimits {
    /// Maximum dimension for 1D textures.
    uint32_t max_texture_dimension_1d;
    /// Maximum dimensions for 2D textures.
    uint32_t max_texture_dimension_2d;
    /// Maximum dimensions for 3D textures.
    uint32_t max_texture_dimension_3d;
    /// Maximum dimensions for cube textures.
    uint32_t max_texture_dimension_cube;
    /// Maximum number of texture layers.
    uint32_t max_texture_array_layers;

    /// Maximum number of vertex input elements in a graphics pipeline.
    uint32_t max_vertex_input_elements;
    /// Maximum offset of a vertex input element in the vertex stream.
    uint32_t max_vertex_input_element_offset;
    /// Maximum number of vertex streams in a graphics pipeline.
    uint32_t max_vertex_streams;
    /// Maximum stride of a vertex stream.
    uint32_t max_vertex_stream_stride;

    /// Maximum number of threads per thread group.
    uint32_t max_compute_threads_per_group;
    /// Maximum dimensions of a thread group.
    uint3 max_compute_thread_group_size;
    /// Maximum number of thread groups per dimension in a single dispatch.
    uint3 max_compute_dispatch_thread_groups;

    /// Maximum number of viewports per pipeline.
    uint32_t max_viewports;
    /// Maximum viewport dimensions.
    uint2 max_viewport_dimensions;
    /// Maximum framebuffer dimensions.
    uint3 max_framebuffer_dimensions;

    /// Maximum samplers visible in a shader stage.
    uint32_t max_shader_visible_samplers;

    std::string to_string() const;
};

struct SGL_API DeviceInfo {
    GraphicsAPI api;

    DeviceLimits limits;
    DeviceFeatures features;
    std::vector<std::string> extended_features;

    /// The name of the graphics adapter.
    std::string adapter_name;

    /// The clock frequency used in timestamp queries.
    uint64_t timestamp_frequency{0};

    std::string to_string() const;
};

class Device : public Object {
    SGL_OBJECT(Device)
public:
    virtual const DeviceDesc& desc() const = 0;
    virtual const DeviceInfo& info() const = 0;

    // virtual SLANG_NO_THROW bool SLANG_MCALL hasFeature(const char* feature) = 0;

    /// Create a new heap resource.
    virtual ref<Heap> create_heap(const HeapDesc& desc) = 0;

    /// Creates a new buffer resource.
    virtual ref<Buffer> create_buffer(const BufferDesc& desc) = 0;

    /// Creates a new buffer on a heap.
    virtual ref<Buffer> create_buffer_on_heap(const BufferDesc& desc, Heap* heap, uint64_t offset) = 0;
    virtual SizeAndAlign get_buffer_size_and_align(const BufferDesc& desc) = 0;

    virtual void* map_buffer(Buffer* buffer, BufferRange range = ENTIRE_BUFFER) = 0;
    virtual void unmap_buffer(Buffer* buffer) = 0;

    /// Creates a new texture resource.
    virtual ref<Texture> create_texture(const TextureDesc& desc) = 0;

    /// Creates a new texture on a heap.
    virtual ref<Texture> create_texture_on_heap(const TextureDesc& desc, Heap* heap, uint64_t offset) = 0;
    virtual SizeAndAlign get_texture_size_and_align(const TextureDesc& desc) = 0;

    /// Creates a new sampler.
    virtual ref<Sampler> create_sampler(const SamplerDesc& desc) = 0;

    /// Creates a new fence.
    // virtual ref<Fence> create_fence(const FenceDesc& desc) = 0;

    // virtual ref<InputLayout> create_input_layout(const InputLayoutDesc& desc) = 0;

    // virtual ref<Framebuffer> create_framebuffer(const FramebufferDesc& desc) = 0;

    // virtual ref<ShaderProgram> create_shader_program(const ShaderProgramDesc& desc) = 0;

    // virtual ref<ComputePipeline> create_compute_pipeline(const ComputePipelineDesc& desc) = 0;

    // virtual ref<GraphicsPipeline> create_graphics_pipeline(const GraphicsPipelineDesc& desc) = 0;

    // virtual ref<RaytracingPipeline> create_raytracing_pipeline(const RaytracingPipelineDesc& desc) = 0;

    // virtual ref<CommandList> create_command_list(const CommandListDesc& desc) = 0;


    // TODO add access mode
    // virtual SLANG_NO_THROW Result SLANG_MCALL mapBuffer(IBuffer* buffer, void** outPointer) = 0;
    // inline void* mapBuffer(IBuffer* buffer)
    // {
    //     void* ptr = nullptr;
    //     SLANG_RETURN_NULL_ON_FAIL(mapBuffer(buffer, &ptr));
    //     return ptr;
    // }

    // virtual SLANG_NO_THROW Result SLANG_MCALL unmapBuffer(IBuffer* buffer) = 0;


    // virtual uint64_t executeCommandLists(ICommandList* const* pCommandLists, size_t numCommandLists, CommandQueue
    // executionQueue = CommandQueue::Graphics) = 0; virtual void queueWaitForCommandList(CommandQueue waitQueue,
    // CommandQueue executionQueue, uint64_t instance) = 0;

    // Wait until all work on the device has completed.
    // virtual SLANG_NO_THROW Result SLANG_MCALL waitForIdle() = 0;

    // Releases the resources that were referenced in the command lists that have finished executing.
    // IMPORTANT: Call this method at least once per frame.
    // virtual SLANG_NO_THROW void SLANG_MCALL runGarbageCollection() = 0;
};

// -----------------------------------------------------------------------------
// Adapter
// -----------------------------------------------------------------------------

using AdapterLUID = std::array<uint8_t, 16>;

struct SGL_API AdapterInfo {
    /// Descriptive name of the adapter.
    std::string name;

    /// The underlying graphics API used by the adapter.
    GraphicsAPI api;

    /// Unique identifier for the vendor.
    /// Only available for D3D12 and Vulkan.
    uint32_t vendor_id;

    /// Unique identifier for the physical device among devices from the vendor.
    /// Only available for D3D12 and Vulkan.
    uint32_t device_id;

    /// Logically unique identifier of the adapter.
    AdapterLUID luid;

    std::string to_string() const;
};

class Adapter : public Object {
    SGL_OBJECT(Adapter)
public:
    virtual const AdapterInfo& info() const = 0;

    virtual ref<Device> create_device(const DeviceDesc& desc) = 0;
};

/// Enumerates all available adapters on the system for the given graphics API.
SGL_API std::vector<ref<Adapter>> enum_adapters(GraphicsAPI api = GraphicsAPI::automatic);

/// Returns the default adapter for the given graphics API.
SGL_API ref<Adapter> default_adapter(GraphicsAPI api = GraphicsAPI::automatic);

/// Creates a new device.
/// If no adapter is provided, the default adapter will be used.
SGL_API ref<Device> create_device(const DeviceDesc& desc, Adapter* adapter = nullptr);

// -----------------------------------------------------------------------------
// Message callback
// -----------------------------------------------------------------------------

enum class MessageSeverity : uint8_t {
    info,
    warning,
    error,
    fatal,
};

// class IMessageCallback {
// public:
//     virtual void SLANG_MCALL message(MessageSeverity severity, const char* msg) = 0;
// };

} // namespace sgl::rhi

#ifdef SLANG_RHI_D3D12
#ifndef SLANG_RHI_H_D3D12
#define SLANG_RHI_H_D3D12
#include <d3d12.h>
namespace slang::rhi {
struct DeviceDescD3D12 {
    StructType structType = StructType::DeviceDescD3D12_1;
    void* next = nullptr;

    // Existing D3D12 objects to use.
    ID3D12Device* device = nullptr;
    ID3D12CommandQueue* graphicsCommandQueue = nullptr;
    ID3D12CommandQueue* computeCommandQueue = nullptr;
    ID3D12CommandQueue* copyCommandQueue = nullptr;

    // Heap sizes.
    uint32_t renderTargetViewHeapSize = 1024;
    uint32_t depthStencilViewHeapSize = 1024;
    uint32_t shaderResourceViewHeapSize = 16384;
    uint32_t samplerHeapSize = 1024;
    uint32_t maxTimerQueries = 256;
};

class IDeviceD3D12 : public ISlangUnknown {
public:
    DXGI_FORMAT getDxgiFormat(Format format);
};
} // namespace slang::rhi
#endif // SLANG_RHI_H_D3D12
#endif // SLANG_RHI_D3D12

#ifdef SLANG_RHI_VULKAN
#ifndef SLANG_RHI_H_VULKAN
#define SLANG_RHI_H_VULKAN
#include <vulkan/vulkan.h>
namespace slang::rhi {
struct DeviceDescVulkan {
    StructType structType = StructType::DeviceDescVulkan_1;
    void* next = nullptr;

    // Existing Vulkan objects to use.
    VkInstance instance = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
    VkQueue graphicsQueue = VK_NULL_HANDLE;
    int graphicsQueueIndex = -1;
    VkQueue transferQueue = VK_NULL_HANDLE;
    int transferQueueIndex = -1;
    VkQueue computeQueue = VK_NULL_HANDLE;
    int computeQueueIndex = -1;

    VkAllocationCallbacks* allocationCallbacks = nullptr;

    const char** instanceExtensions = nullptr;
    size_t instanceExtensionCount = 0;

    const char** deviceExtensions = nullptr;
    size_t deviceExtensionCount = 0;

    uint32_t maxTimerQueries = 256;
};

class IDeviceVulkan : public ISlangUnknown {
public:
    VkFormat getVkFormat(Format format);
};
} // namespace slang::rhi
#endif // SLANG_RHI_H_VULKAN
#endif // SLANG_RHI_VULKAN
