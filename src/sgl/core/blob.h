#pragma once

#include "sgl/core/object.h"

#include <memory>

namespace sgl {

class SGL_API Blob : public Object {
    SGL_OBJECT(Blob)
public:
    Blob() = default;
    explicit Blob(size_t size);
    explicit Blob(const void* data, size_t size);
    Blob(const Blob& other);
    Blob(Blob&& other) noexcept;

    Blob& operator=(const Blob& other);
    Blob& operator=(Blob&& other) noexcept;

    size_t size() const { return m_size; }

    const uint8_t* data() const { return m_data.get(); }
    uint8_t* data() { return m_data.get(); }

    template<typename T>
    const T* data_as() const
    {
        return reinterpret_cast<const T*>(m_data.get());
    }

    template<typename T>
    T* data_as()
    {
        return reinterpret_cast<T*>(m_data.get());
    }

private:
    size_t m_size{0};
    std::unique_ptr<uint8_t[]> m_data;
};

} // namespace sgl
