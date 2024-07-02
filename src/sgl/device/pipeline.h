// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "sgl/device/fwd.h"
#include "sgl/device/device_resource.h"
#include "sgl/device/types.h"
#include "sgl/device/native_handle.h"

#include "sgl/core/macros.h"
#include "sgl/core/enum.h"
#include "sgl/core/object.h"

#include "sgl/math/vector_types.h"

#include <slang-gfx.h>

#include <map>

namespace sgl {

/// Pipeline base class.
class SGL_API Pipeline : public DeviceResource {
    SGL_OBJECT(Pipeline)
public:
    Pipeline(ref<Device> device);

    virtual ~Pipeline();

    gfx::IPipelineState* gfx_pipeline_state() const { return m_gfx_pipeline_state; }

    /// Returns the native API handle:
    /// - D3D12: ID3D12PipelineState*
    /// - Vulkan: VkPipeline
    NativeHandle get_native_handle() const;

protected:
    Slang::ComPtr<gfx::IPipelineState> m_gfx_pipeline_state;
};

struct ComputePipelineDesc {
    ref<ShaderProgram> program;
};

/// Compute pipeline.
class SGL_API ComputePipeline : public Pipeline {
public:
    ComputePipeline(ref<Device> device, ComputePipelineDesc desc);

    /// Thread group size.
    /// Used to determine the number of thread groups to dispatch.
    uint3 thread_group_size() const { return m_thread_group_size; }

    std::string to_string() const override;

private:
    /// Shared reference to shader program to keep reflection data alive.
    ref<ShaderProgram> m_program;
    uint3 m_thread_group_size;
};

struct GraphicsPipelineDesc {
    ref<ShaderProgram> program;
    ref<InputLayout> input_layout;
    ref<FramebufferLayout> framebuffer_layout;
    PrimitiveType primitive_type{PrimitiveType::triangle};
    DepthStencilDesc depth_stencil;
    RasterizerDesc rasterizer;
    BlendDesc blend;
};

/// Graphics pipeline.
class SGL_API GraphicsPipeline : public Pipeline {
public:
    GraphicsPipeline(ref<Device> device, GraphicsPipelineDesc desc);

    std::string to_string() const override;

private:
    /// Shared reference to shader program to keep reflection data alive.
    ref<ShaderProgram> m_program;
};

struct HitGroupDesc {
    std::string hit_group_name;
    std::string closest_hit_entry_point;
    std::string any_hit_entry_point;
    std::string intersection_entry_point;
};

struct RayTracingPipelineDesc {
    ref<ShaderProgram> program;
    std::vector<HitGroupDesc> hit_groups;
    uint32_t max_recursion{0};
    uint32_t max_ray_payload_size{0};
    uint32_t max_attribute_size{8};
    RayTracingPipelineFlags flags{RayTracingPipelineFlags::none};
};

/// Ray tracing pipeline.
class SGL_API RayTracingPipeline : public Pipeline {
public:
    RayTracingPipeline(ref<Device> device, RayTracingPipelineDesc desc);

    std::string to_string() const override;

private:
    /// Shared reference to shader program to keep reflection data alive.
    ref<ShaderProgram> m_program;
};

} // namespace sgl
