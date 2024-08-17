#pragma once

#include "rhi.h"

namespace sgl::rhi {

class HeapBase : public Heap {
public:
    HeapBase(const HeapDesc& desc)
        : m_desc(desc)
    {
    }

    virtual const HeapDesc& desc() const override { return m_desc; }

    virtual std::string to_string() const override;

    HeapDesc m_desc;
};

class BufferBase : public Buffer {
public:
    BufferBase(const BufferDesc& desc)
        : m_desc(desc)
    {
    }

    virtual const BufferDesc& desc() const override { return m_desc; }

    virtual std::string to_string() const override;

    BufferDesc m_desc;
};

class TextureBase : public Texture {
public:
    TextureBase(const TextureDesc& desc)
        : m_desc(desc)
    {
    }

    virtual const TextureDesc& desc() const override { return m_desc; }

    virtual std::string to_string() const override;

    TextureDesc m_desc;
};

class SamplerBase : public Sampler {
public:
    SamplerBase(const SamplerDesc& desc)
        : m_desc(desc)
    {
    }

    virtual const SamplerDesc& desc() const override { return m_desc; }

    virtual std::string to_string() const override;

    SamplerDesc m_desc;
};

class DeviceBase : public Device {
public:
    DeviceBase(const DeviceDesc& desc)
        : m_desc(desc)
    {
    }

    virtual const DeviceDesc& desc() const override { return m_desc; }
    virtual const DeviceInfo& info() const override { return m_info; }

    DeviceDesc m_desc;
    DeviceInfo m_info;
};

class AdapterBase : public Adapter {
public:
    virtual const AdapterInfo& info() const override { return m_info; }

    AdapterInfo m_info;
};


} // namespace sgl::rhi
