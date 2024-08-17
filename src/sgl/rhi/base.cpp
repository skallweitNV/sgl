#include "base.h"

#include "sgl/core/format.h"
#include "sgl/math/vector_math.h"

namespace sgl::rhi {

std::string HeapBase::to_string() const
{
    return fmt::format(
        "Heap(\n"
        "  size = {},\n"
        "  memory_type = {},\n"
        "  debug_name = \"{}\",\n"
        ")",
        m_desc.size,
        m_desc.memory_type,
        m_desc.debug_name
    );
}

std::string BufferBase::to_string() const
{
    return fmt::format(
        "Buffer(\n"
        "  size = {},\n"
        "  format = {},\n"
        "  debug_name = \"{}\",\n"
        "  memory_type = {},\n"
        "  default_state = {},\n"
        "  allowed_states = {},\n"
        "  is_shared = {},\n"
        "  device_address = {},\n"
        ")",
        m_desc.size,
        m_desc.format,
        m_desc.debug_name,
        m_desc.memory_type,
        m_desc.default_state,
        m_desc.allowed_states,
        m_desc.is_shared,
        device_address()
    );
}

std::string TextureBase::to_string() const
{
    return "Texture";
}

std::string SamplerBase::to_string() const
{
    return fmt::format(
        "Sampler(\n"
        "  min_filter = {},\n"
        "  mag_filter = {},\n"
        "  mip_filter = {},\n"
        "  reduction_op = {},\n"
        "  address_u = {},\n"
        "  address_v = {},\n"
        "  address_w = {},\n"
        "  mip_bias = {},\n"
        "  max_anisotropy = {},\n"
        "  comparison_func = {},\n"
        "  border_color = {},\n"
        "  mip_min = {},\n"
        "  mip_max = {}\n"
        ")",
        m_desc.min_filter,
        m_desc.mag_filter,
        m_desc.mip_filter,
        m_desc.reduction_op,
        m_desc.address_u,
        m_desc.address_v,
        m_desc.address_w,
        m_desc.mip_bias,
        m_desc.max_anisotropy,
        m_desc.comparison_func,
        m_desc.border_color,
        m_desc.mip_min,
        m_desc.mip_max
    );
}

} // namespace sgl::rhi
