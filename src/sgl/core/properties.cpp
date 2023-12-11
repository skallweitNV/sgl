#include "properties.h"

#include <map>
#include <variant>
#include <functional>

namespace kali {

using ValueData = std::variant<bool, int64_t, uint64_t, double, std::string>;

struct Value {
    Properties::Type type;
    ValueData data;
};

struct Properties::Impl {
    Impl() = default;
    Impl(const Impl&) = default;

    std::map<std::string, Value, std::less<>> values;
};

Properties::Properties()
{
    m_impl = std::make_unique<Impl>();
}

Properties::Properties(const Properties& other)
{
    m_impl = std::make_unique<Impl>(*other.m_impl);
}

Properties::Properties(Properties&& other)
{
    m_impl = std::exchange(other.m_impl, nullptr);
}

Properties::~Properties() { }

Properties& Properties::operator=(const Properties& other)
{
    if (this != &other) {
        *m_impl = *other.m_impl;
    }
    return *this;
}

Properties& Properties::operator=(Properties&& other)
{
    if (this != &other) {
        m_impl = std::exchange(other.m_impl, nullptr);
    }
    return *this;
}

bool Properties::empty() const
{
    return m_impl->values.empty();
}

bool Properties::has(std::string_view name) const
{
    return m_impl->values.find(name) != m_impl->values.end();
}

template<typename T, Properties::Type ValueType>
void Properties::set_impl(std::string_view name, const T& value)
{
    auto it = m_impl->values.find(name);
    if (it != m_impl->values.end()) {
        it->second = value;
    } else {
        m_impl->values.emplace(std::string{name}, value);
    }
}

template<typename T, Properties::Type ValueType>
bool Properties::get_impl(std::string_view name, T& value) const
{
    auto it = m_impl->values.find(name);
    if (it != m_impl->values.end()) {
        value = std::get<T>(it->second);
        return true;
    } else {
        return false;
    }
}

#define EXPORT_PROPERTY_ACCESSOR(T, ValueType)                                                                         \
    template KALI_API void Properties::set_impl<T, ValueType>(std::string_view, const T&);                             \
    template KALI_API bool Properties::get_impl<T, ValueType>(std::string_view, T&) const;

EXPORT_PROPERTY_ACCESSOR(bool, Properties::Type::bool_)
EXPORT_PROPERTY_ACCESSOR(int32_t, Properties::Type::int_)
EXPORT_PROPERTY_ACCESSOR(int64_t, Properties::Type::int_)
EXPORT_PROPERTY_ACCESSOR(uint32_t, Properties::Type::uint_)
EXPORT_PROPERTY_ACCESSOR(uint64_t, Properties::Type::uint_)
EXPORT_PROPERTY_ACCESSOR(float, Properties::Type::float_)
EXPORT_PROPERTY_ACCESSOR(double, Properties::Type::float_)
EXPORT_PROPERTY_ACCESSOR(std::string, Properties::Type::string)
EXPORT_PROPERTY_ACCESSOR(std::filesystem::path, Properties::Type::string)
// EXPORT_PROPERTY_ACCESSOR(int2)
// EXPORT_PROPERTY_ACCESSOR(int3)
// EXPORT_PROPERTY_ACCESSOR(int4)
// EXPORT_PROPERTY_ACCESSOR(uint2)
// EXPORT_PROPERTY_ACCESSOR(uint3)
// EXPORT_PROPERTY_ACCESSOR(uint4)
// EXPORT_PROPERTY_ACCESSOR(float2)
// EXPORT_PROPERTY_ACCESSOR(float3)
// EXPORT_PROPERTY_ACCESSOR(float4)
// EXPORT_PROPERTY_ACCESSOR(Properties)

#undef EXPORT_PROPERTY_ACCESSOR

} // namespace kali
