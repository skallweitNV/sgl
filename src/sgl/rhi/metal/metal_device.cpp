#include "metal_backend.h"

namespace sgl::rhi::metal {

DeviceImpl::DeviceImpl(const DeviceDesc& desc, AdapterImpl* adapter)
    : DeviceBase(desc)
{
    m_info.api = GraphicsAPI::metal;
    m_info.limits = {};
    m_info.features = DeviceFeatures(0);
    m_info.extended_features = {};

    // TODO use correct device
    m_ctx.device = NS::TransferPtr(MTL::CreateSystemDefaultDevice());

    m_ctx.device->supportsRaytracing();
}

DeviceImpl::~DeviceImpl() { }

} // namespace sgl::rhi::metal
