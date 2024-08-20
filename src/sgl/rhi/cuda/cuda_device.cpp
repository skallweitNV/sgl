#include "cuda_backend.h"

namespace sgl::rhi::cuda {

DeviceImpl::DeviceImpl(const DeviceDesc& desc, AdapterImpl* adapter)
    : DeviceBase(desc)
{
    m_info.api = GraphicsAPI::cuda;
    m_info.limits = {};
    m_info.features = DeviceFeatures(0);
    m_info.extended_features = {};
    m_info.adapter_name = adapter->m_info.name;

    m_ctx.cu_device = adapter->cu_device;
}

DeviceImpl::~DeviceImpl() { }

} // namespace sgl::rhi::cuda
