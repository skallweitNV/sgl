#include "blob.h"

namespace sgl {

Blob::Blob(size_t size)
    : m_size(size)
    , m_data(new uint8_t[size])
{
}

Blob::Blob(const void* data, size_t size)
    : m_size(size)
    , m_data(new uint8_t[size])
{
    std::memcpy(m_data.get(), data, size);
}

Blob::Blob(const Blob& other)
    : m_size(other.m_size)
    , m_data(new uint8_t[other.m_size])
{
    std::memcpy(m_data.get(), other.m_data.get(), other.m_size);
}

Blob::Blob(Blob&& other) noexcept
    : m_size(std::exchange(other.m_size, 0))
    , m_data(std::exchange(other.m_data, nullptr))
{
}

Blob& Blob::operator=(const Blob& other)
{
    m_data.reset();
    m_size = other.m_size;
    m_data = std::make_unique<uint8_t[]>(other.m_size);
    std::memcpy(m_data.get(), other.m_data.get(), other.m_size);
    return *this;
}

Blob& Blob::operator=(Blob&& other) noexcept
{
    m_data.reset();
    m_size = std::exchange(other.m_size, 0);
    m_data = std::exchange(other.m_data, nullptr);
    return *this;
}

} // namespace sgl
