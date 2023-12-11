#pragma once

#include "kali/core/object.h"
#include "kali/core/enum.h"
#include "kali/math/vector_types.h"
#include "kali/math/matrix_types.h"

#include <memory>
#include <string_view>
#include <filesystem>

namespace kali {

class Properties : public Object {
    KALI_OBJECT(Properties)
public:
    enum class Type {
        bool_,
        int_,
        uint_,
        float_,
        string,
    };

    KALI_ENUM_INFO(
        Type,
        {
            {Type::bool_, "bool"},
            {Type::int_, "int"},
            {Type::uint_, "uint"},
            {Type::float_, "float"},
            {Type::string, "string"},
        }
    );

    Properties();

    Properties(const Properties& other);
    Properties(Properties&& other);

    ~Properties();

    Properties& operator=(const Properties& other);
    Properties& operator=(Properties&& other);

    /// Check if the properties are empty.
    bool empty() const;

    /// Check if a property exists.
    bool has(std::string_view name) const;

    /// Set a property.
    template<typename T>
    void set(std::string_view name, const T& value)
    {
        if constexpr (has_enum_info_v<T>) {
            set_impl(name, enum_to_string(value));
            // } else if constexpr (detail::has_serialize_v<T>) {
            //     set_impl(name, serializeToProperties(value));
        } else {
            set_impl<T>(name, value);
        }
    }

    /// Get a property.
    /// Throws if property does not exist or has the wrong type.
    template<typename T>
    T get(std::string_view name) const
    {
        if constexpr (has_enum_info_v<T>) {
            std::string value;
            if (!get_impl(name, value))
                KALI_THROW("Property '{}' does not exist.", name);
            return stringToEnum<T>(value);
            // } else if constexpr (detail::has_serialize_v<T>) {
            //     Properties props;
            //     if (!get_impl(name, props))
            //         KALI_THROW("Property '{}' does not exist.", name);
            //     return deserializeFromProperties<T>(props);
        } else {
            T value;
            if (!get_impl(name, value))
                KALI_THROW("Property '{}' does not exist.", name);
            return value;
        }
    }

private:
    template<typename T>
    void set_impl(std::string_view name, const T& value);

    template<typename T>
    bool get_impl(std::string_view name, T& value) const;


    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

KALI_ENUM_REGISTER(Properties::Type);

#define EXTERN_PROPERTY_ACCESSOR(T)                                                                                    \
    extern template KALI_API void Properties::set_impl<T>(std::string_view, const T&);                                 \
    extern template KALI_API bool Properties::get_impl<T>(std::string_view, T&) const;

EXTERN_PROPERTY_ACCESSOR(bool)
EXTERN_PROPERTY_ACCESSOR(int32_t)
EXTERN_PROPERTY_ACCESSOR(int64_t)
EXTERN_PROPERTY_ACCESSOR(uint32_t)
EXTERN_PROPERTY_ACCESSOR(uint64_t)
EXTERN_PROPERTY_ACCESSOR(float)
EXTERN_PROPERTY_ACCESSOR(double)
EXTERN_PROPERTY_ACCESSOR(std::string)
EXTERN_PROPERTY_ACCESSOR(std::filesystem::path)
EXTERN_PROPERTY_ACCESSOR(int2)
EXTERN_PROPERTY_ACCESSOR(int3)
EXTERN_PROPERTY_ACCESSOR(int4)
EXTERN_PROPERTY_ACCESSOR(uint2)
EXTERN_PROPERTY_ACCESSOR(uint3)
EXTERN_PROPERTY_ACCESSOR(uint4)
EXTERN_PROPERTY_ACCESSOR(float2)
EXTERN_PROPERTY_ACCESSOR(float3)
EXTERN_PROPERTY_ACCESSOR(float4)
// EXTERN_PROPERTY_ACCESSOR(Properties)

} // namespace kali
