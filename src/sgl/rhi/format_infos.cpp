#include "rhi.h"

#include "sgl/core/error.h"

namespace sgl::rhi {

static const FormatInfo FORMAT_INFOS[] = {
    // clang-format off
    // format                   name                    kind                        bpb     bs      red     green   blue    alpha   depth   stencil signed  srgb
    {Format::unknown,           "unknown",              FormatKind::integer,        0,      0,      false,  false,  false,  false,  false,  false,  false,  false   },
    {Format::r8_uint,           "r8_uint",              FormatKind::integer,        1,      1,      true,   false,  false,  false,  false,  false,  false,  false   },
    {Format::r8_sint,           "r8_sint",              FormatKind::integer,        1,      1,      true,   false,  false,  false,  false,  false,  true,   false   },
    {Format::r8_unorm,          "r8_unorm",             FormatKind::normalized,     1,      1,      true,   false,  false,  false,  false,  false,  false,  false   },
    {Format::r8_snorm,          "r8_snorm",             FormatKind::normalized,     1,      1,      true,   false,  false,  false,  false,  false,  true,   false   },
    {Format::rg8_uint,          "rg8_uint",             FormatKind::integer,        2,      1,      true,   true,   false,  false,  false,  false,  false,  false   },
    {Format::rg8_sint,          "rg8_sint",             FormatKind::integer,        2,      1,      true,   true,   false,  false,  false,  false,  true,   false   },
    {Format::rg8_unorm,         "rg8_unorm",            FormatKind::normalized,     2,      1,      true,   true,   false,  false,  false,  false,  false,  false   },
    {Format::rg8_snorm,         "rg8_snorm",            FormatKind::normalized,     2,      1,      true,   true,   false,  false,  false,  false,  true,   false   },
    {Format::r16_uint,          "r16_uint",             FormatKind::integer,        2,      1,      true,   false,  false,  false,  false,  false,  false,  false   },
    {Format::r16_sint,          "r16_sint",             FormatKind::integer,        2,      1,      true,   false,  false,  false,  false,  false,  true,   false   },
    {Format::r16_unorm,         "r16_unorm",            FormatKind::normalized,     2,      1,      true,   false,  false,  false,  false,  false,  false,  false   },
    {Format::r16_snorm,         "r16_snorm",            FormatKind::normalized,     2,      1,      true,   false,  false,  false,  false,  false,  true,   false   },
    {Format::r16_float,         "r16_float",            FormatKind::float_,         2,      1,      true,   false,  false,  false,  false,  false,  true,   false   },
    {Format::bgra4_unorm,       "bgra4_unorm",          FormatKind::normalized,     2,      1,      true,   true,   true,   true,   false,  false,  false,  false   },
    {Format::b5g6r5_unorm,      "b5g6r5_unorm",         FormatKind::normalized,     2,      1,      true,   true,   true,   false,  false,  false,  false,  false   },
    {Format::b5g5r5a1_unorm,    "b5g5r5a1_unorm",       FormatKind::normalized,     2,      1,      true,   true,   true,   true,   false,  false,  false,  false   },
    {Format::rgba8_uint,        "rgba8_uint",           FormatKind::integer,        4,      1,      true,   true,   true,   true,   false,  false,  false,  false   },
    {Format::rgba8_sint,        "rgba8_sint",           FormatKind::integer,        4,      1,      true,   true,   true,   true,   false,  false,  true,   false   },
    {Format::rgba8_unorm,       "rgba8_unorm",          FormatKind::normalized,     4,      1,      true,   true,   true,   true,   false,  false,  false,  false   },
    {Format::rgba8_snorm,       "rgba8_snorm",          FormatKind::normalized,     4,      1,      true,   true,   true,   true,   false,  false,  true,   false   },
    {Format::bgra8_unorm,       "bgra8_unorm",          FormatKind::normalized,     4,      1,      true,   true,   true,   true,   false,  false,  false,  false   },
    {Format::srgba8_unorm,      "srgba8_unorm",         FormatKind::normalized,     4,      1,      true,   true,   true,   true,   false,  false,  false,  true    },
    {Format::sbgra8_unorm,      "sbgra8_unorm",         FormatKind::normalized,     4,      1,      true,   true,   true,   true,   false,  false,  false,  false   },
    {Format::r10g10b10a2_unorm, "r10g10b10a2_unorm",    FormatKind::normalized,     4,      1,      true,   true,   true,   true,   false,  false,  false,  false   },
    {Format::r11g11b10_float,   "r11g11b10_float",      FormatKind::float_,         4,      1,      true,   true,   true,   false,  false,  false,  false,  false   },
    {Format::rg16_uint,         "rg16_uint",            FormatKind::integer,        4,      1,      true,   true,   false,  false,  false,  false,  false,  false   },
    {Format::rg16_sint,         "rg16_sint",            FormatKind::integer,        4,      1,      true,   true,   false,  false,  false,  false,  true,   false   },
    {Format::rg16_unorm,        "rg16_unorm",           FormatKind::normalized,     4,      1,      true,   true,   false,  false,  false,  false,  false,  false   },
    {Format::rg16_snorm,        "rg16_snorm",           FormatKind::normalized,     4,      1,      true,   true,   false,  false,  false,  false,  true,   false   },
    {Format::rg16_float,        "rg16_float",           FormatKind::float_,         4,      1,      true,   true,   false,  false,  false,  false,  true,   false   },
    {Format::r32_uint,          "r32_uint",             FormatKind::integer,        4,      1,      true,   false,  false,  false,  false,  false,  false,  false   },
    {Format::r32_sint,          "r32_sint",             FormatKind::integer,        4,      1,      true,   false,  false,  false,  false,  false,  true,   false   },
    {Format::r32_float,         "r32_float",            FormatKind::float_,         4,      1,      true,   false,  false,  false,  false,  false,  true,   false   },
    {Format::rgba16_uint,       "rgba16_uint",          FormatKind::integer,        8,      1,      true,   true,   true,   true,   false,  false,  false,  false   },
    {Format::rgba16_sint,       "rgba16_sint",          FormatKind::integer,        8,      1,      true,   true,   true,   true,   false,  false,  true,   false   },
    {Format::rgba16_float,      "rgba16_float",         FormatKind::float_,         8,      1,      true,   true,   true,   true,   false,  false,  true,   false   },
    {Format::rgba16_unorm,      "rgba16_unorm",         FormatKind::normalized,     8,      1,      true,   true,   true,   true,   false,  false,  false,  false   },
    {Format::rgba16_snorm,      "rgba16_snorm",         FormatKind::normalized,     8,      1,      true,   true,   true,   true,   false,  false,  true,   false   },
    {Format::rg32_uint,         "rg32_uint",            FormatKind::integer,        8,      1,      true,   true,   false,  false,  false,  false,  false,  false   },
    {Format::rg32_sint,         "rg32_sint",            FormatKind::integer,        8,      1,      true,   true,   false,  false,  false,  false,  true,   false   },
    {Format::rg32_float,        "rg32_float",           FormatKind::float_,         8,      1,      true,   true,   false,  false,  false,  false,  true,   false   },
    {Format::rgb32_uint,        "rgb32_uint",           FormatKind::integer,        12,     1,      true,   true,   true,   false,  false,  false,  false,  false   },
    {Format::rgb32_sint,        "rgb32_sint",           FormatKind::integer,        12,     1,      true,   true,   true,   false,  false,  false,  true,   false   },
    {Format::rgb32_float,       "rgb32_float",          FormatKind::float_,         12,     1,      true,   true,   true,   false,  false,  false,  true,   false   },
    {Format::rgba32_uint,       "rgba32_uint",          FormatKind::integer,        16,     1,      true,   true,   true,   true,   false,  false,  false,  false   },
    {Format::rgba32_sint,       "rgba32_sint",          FormatKind::integer,        16,     1,      true,   true,   true,   true,   false,  false,  true,   false   },
    {Format::rgba32_float,      "rgba32_float",         FormatKind::float_,         16,     1,      true,   true,   true,   true,   false,  false,  true,   false   },
    {Format::d16,               "d16",                  FormatKind::depth_stencil,  2,      1,      false,  false,  false,  false,  true,   false,  false,  false   },
    {Format::d24s8,             "d24s8",                FormatKind::depth_stencil,  4,      1,      false,  false,  false,  false,  true,   true,   false,  false   },
    {Format::x24g8_uint,        "x24g8_uint",           FormatKind::integer,        4,      1,      false,  false,  false,  false,  false,  true,   false,  false   },
    {Format::d32,               "d32",                  FormatKind::depth_stencil,  4,      1,      false,  false,  false,  false,  true,   false,  false,  false   },
    {Format::d32s8,             "d32s8",                FormatKind::depth_stencil,  8,      1,      false,  false,  false,  false,  true,   true,   false,  false   },
    {Format::x32g8_uint,        "x32g8_uint",           FormatKind::integer,        8,      1,      false,  false,  false,  false,  false,  true,   false,  false   },
    {Format::bc1_unorm,         "bc1_unorm",            FormatKind::normalized,     8,      4,      true,   true,   true,   true,   false,  false,  false,  false   },
    {Format::bc1_unorm_srgb,    "bc1_unorm_srgb",       FormatKind::normalized,     8,      4,      true,   true,   true,   true,   false,  false,  false,  true    },
    {Format::bc2_unorm,         "bc2_unorm",            FormatKind::normalized,     16,     4,      true,   true,   true,   true,   false,  false,  false,  false   },
    {Format::bc2_unorm_srgb,    "bc2_unorm_srgb",       FormatKind::normalized,     16,     4,      true,   true,   true,   true,   false,  false,  false,  true    },
    {Format::bc3_unorm,         "bc3_unorm",            FormatKind::normalized,     16,     4,      true,   true,   true,   true,   false,  false,  false,  false   },
    {Format::bc3_unorm_srgb,    "bc3_unorm_srgb",       FormatKind::normalized,     16,     4,      true,   true,   true,   true,   false,  false,  false,  true    },
    {Format::bc4_unorm,         "bc4_unorm",            FormatKind::normalized,     8,      4,      true,   false,  false,  false,  false,  false,  false,  false   },
    {Format::bc4_snorm,         "bc4_snorm",            FormatKind::normalized,     8,      4,      true,   false,  false,  false,  false,  false,  true,   false   },
    {Format::bc5_unorm,         "bc5_unorm",            FormatKind::normalized,     16,     4,      true,   true,   false,  false,  false,  false,  false,  false   },
    {Format::bc5_snorm,         "bc5_snorm",            FormatKind::normalized,     16,     4,      true,   true,   false,  false,  false,  false,  true,   false   },
    {Format::bc6h_ufloat,       "bc6h_ufloat",          FormatKind::float_,         16,     4,      true,   true,   true,   false,  false,  false,  false,  false   },
    {Format::bc6h_sfloat,       "bc6h_sfloat",          FormatKind::float_,         16,     4,      true,   true,   true,   false,  false,  false,  true,   false   },
    {Format::bc7_unorm,         "bc7_unorm",            FormatKind::normalized,     16,     4,      true,   true,   true,   true,   false,  false,  false,  false   },
    {Format::bc7_unorm_srgb,    "bc7_unorm_srgb",       FormatKind::normalized,     16,     4,      true,   true,   true,   true,   false,  false,  false,  true    },
    // clang-format on
};

static_assert(std::size(FORMAT_INFOS) == static_cast<size_t>(Format::COUNT));

const FormatInfo& FormatInfo::get(Format format)
{
    SGL_CHECK(size_t(format) < size_t(Format::COUNT), "Invalid format");
    return FORMAT_INFOS[size_t(format)];
}

std::string FormatInfo::to_string() const
{
    return fmt::format(
        "FormatInfo(\n"
        "  format = {},\n"
        "  name = \"{}\",\n"
        "  kind = {},\n"
        "  bytes_per_block = {},\n"
        "  block_size = {},\n"
        "  has_red = {},\n"
        "  has_green = {},\n"
        "  has_blue = {},\n"
        "  has_alpha = {},\n"
        "  has_depth = {},\n"
        "  has_stencil = {},\n"
        "  is_signed = {},\n"
        "  is_srgb = {},\n"
        ")",
        format,
        name,
        kind,
        bytes_per_block,
        block_size,
        has_red,
        has_green,
        has_blue,
        has_alpha,
        has_depth,
        has_stencil,
        is_signed,
        is_srgb
    );
}

} // namespace sgl::rhi
