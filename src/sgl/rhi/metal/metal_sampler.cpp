#include "metal_backend.h"
#include "metal_utils.h"

namespace sgl::rhi::metal {

SamplerImpl::SamplerImpl(ref<DeviceImpl> device, const SamplerDesc& desc)
    : SamplerBase(desc)
    , device(device)
{
    NS::SharedPtr<MTL::SamplerDescriptor> sampler_desc = NS::TransferPtr(MTL::SamplerDescriptor::alloc()->init());

    sampler_desc->setMinFilter(utils::translate_sampler_min_mag_filter(desc.min_filter));
    sampler_desc->setMagFilter(utils::translate_sampler_min_mag_filter(desc.mag_filter));
    sampler_desc->setMipFilter(utils::translate_sampler_mip_filter(desc.mip_filter));

    sampler_desc->setSAddressMode(utils::translate_sampler_address_mode(desc.address_u));
    sampler_desc->setTAddressMode(utils::translate_sampler_address_mode(desc.address_v));
    sampler_desc->setRAddressMode(utils::translate_sampler_address_mode(desc.address_w));

    sampler_desc->setMaxAnisotropy(std::clamp(desc.max_anisotropy, 1u, 16u));

    // TODO: support translation of border color...
    MTL::SamplerBorderColor border_color = MTL::SamplerBorderColorOpaqueBlack;
    sampler_desc->setBorderColor(border_color);

    sampler_desc->setNormalizedCoordinates(true);

    sampler_desc->setCompareFunction(utils::translate_compare_function(desc.comparison_func));
    sampler_desc->setLodMinClamp(std::clamp(desc.mip_min, 0.f, 1000.f));
    sampler_desc->setLodMaxClamp(std::clamp(desc.mip_max, sampler_desc->lodMinClamp(), 1000.f));

    sampler_desc->setSupportArgumentBuffers(true);

    mtl_sampler_state = NS::TransferPtr(device->m_ctx.device->newSamplerState(sampler_desc.get()));
    SGL_CHECK(mtl_sampler_state, "Failed to create sampler state");
}

SamplerImpl::~SamplerImpl() { }

NativeHandle SamplerImpl::get_native_handle(NativeHandleType type) const
{
    if (type == NativeHandleType::mtl_sampler_state)
        return NativeHandle(mtl_sampler_state.get());
    return {};
}

ref<Sampler> DeviceImpl::create_sampler(const SamplerDesc& desc)
{
    AUTORELEASEPOOL
    return ref(new SamplerImpl(ref(this), desc));
}

} // namespace sgl::rhi::metal
