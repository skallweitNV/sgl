#include "cuda_backend.h"
#include "cuda_utils.h"

#include "sgl/rhi/validation/validation.h"

#include "sgl/core/type_utils.h"

namespace sgl::rhi::cuda {

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

    int device_count;
    SGL_CU_CHECK(cuDeviceGetCount(&device_count));
    for (int device_index = 0; device_index < device_count; ++device_index) {
        ref<AdapterImpl> adapter(new AdapterImpl());
        AdapterInfo& info = adapter->m_info;

        SGL_CU_CHECK(cuDeviceGet(&adapter->cu_device, device_index));

        char device_name[256];
        SGL_CU_CHECK(cuDeviceGetName(device_name, sizeof(device_name), adapter->cu_device));

        info.name = device_name;
        info.api = GraphicsAPI::cuda;
        info.vendor_id = 0;
        info.device_id = 0;
        unsigned int device_node_mask;
        SGL_CU_CHECK(cuDeviceGetLuid(reinterpret_cast<char*>(&info.luid[0]), &device_node_mask, adapter->cu_device));
        adapters.push_back(adapter);
    }

    return adapters;
}

} // namespace sgl::rhi::cuda
