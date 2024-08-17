#include "metal_backend.h"
#include "metal_utils.h"

#include "sgl/rhi/validation/validation.h"

#include "sgl/core/type_utils.h"

namespace sgl::rhi::metal {

ref<Device> AdapterImpl::create_device(const DeviceDesc& desc)
{
    ref<Device> device(new DeviceImpl(desc, this));
    if (desc.enable_validation)
        device = validation::wrap_device(device);
    return device;
}

std::vector<ref<Adapter>> enum_adapters()
{
    AUTORELEASEPOOL
    std::vector<ref<Adapter>> adapters;

    auto add_adapter = [&adapters](MTL::Device* device)
    {
        ref<AdapterImpl> adapter(new AdapterImpl());
        AdapterInfo& info = adapter->m_info;
        info.name = device->name()->cString(NS::ASCIIStringEncoding);
        info.api = GraphicsAPI::metal;
        info.vendor_id = 0;
        info.device_id = 0;
        uint64_t registry_id = device->registryID();
        memcpy(&info.luid[0], &registry_id, sizeof(registry_id));
        adapters.push_back(adapter);
    };

    NS::Array* devices = MTL::CopyAllDevices();
    if (devices->count() > 0) {
        for (int i = 0; i < devices->count(); ++i) {
            MTL::Device* device = static_cast<MTL::Device*>(devices->object(i));
            add_adapter(device);
        }
    } else {
        MTL::Device* device = MTL::CreateSystemDefaultDevice();
        add_adapter(device);
        device->release();
    }

    return adapters;
}

} // namespace sgl::rhi::metal
