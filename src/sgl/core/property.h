
#include "kali/core/macros.h"
#include "kali/math/vector_types.h"

#include <map>
#include <span>
#include <string>
#include <string_view>
#include <vector>
#include <initializer_list>

namespace kali {

class PropertyValue;

template<typename T>
struct PropertyAdapter {
    static bool has_shape(const PropertyValue& p);
    static PropertyValue to_property(const T& v);
    static T from_property(const PropertyValue& p);
};

// concept to require a templated struct with methods has_shape, to_property and from_property
template<typename T>
concept has_property_adapter = requires(T a, const PropertyValue& b) {
    {
        PropertyAdapter<T>::has_shape(b)
    } -> std::convertible_to<bool>;
    {
        PropertyAdapter<T>::to_property(a)
    } -> std::convertible_to<PropertyValue>;
    {
        PropertyAdapter<T>::from_property(b)
    } -> std::convertible_to<T>;
};


#if 0
namespace property {

} // namespace property
#endif

class KALI_API PropertyValue {
public:
    enum class Type {
        null,
        boolean,
        number_signed,
        number_unsigned,
        number_float,
        string,
        object,
        array,
    };

    using map_type = std::map<std::string, PropertyValue, std::less<>>;
    using array_type = std::vector<PropertyValue>;

    PropertyValue(Type type)
        : m_type(type)
    {
        if (type == Type::string)
            m_data.string = new std::string;
        else if (type == Type::object)
            m_data.object = new map_type;
        else if (type == Type::array)
            m_data.array = new array_type;
        else
            m_data.bits = 0;
    }

    /// Copy constructor.
    PropertyValue(const PropertyValue& other)
    {
        m_type = other.m_type;
        if (m_type == Type::string)
            m_data.string = new std::string(*other.m_data.string);
        else if (m_type == Type::object)
            m_data.object = new map_type(*other.m_data.object);
        else if (m_type == Type::array)
            m_data.array = new array_type(*other.m_data.array);
        else
            m_data.bits = other.m_data.bits;
    }

    /// Move constructor.
    PropertyValue(PropertyValue&& other)
    {
        m_type = other.m_type;
        if (m_type == Type::string)
            m_data.string = other.m_data.string;
        else if (m_type == Type::object)
            m_data.object = other.m_data.object;
        else if (m_type == Type::array)
            m_data.array = other.m_data.array;
        else
            m_data.bits = other.m_data.bits;

        other.m_type = Type::null;
        other.m_data.bits = 0;
    }

    /// Copy assignment operator.
    PropertyValue& operator=(const PropertyValue& other)
    {
        if (this == &other)
            return *this;

        if (m_type == Type::string)
            delete m_data.string;
        else if (m_type == Type::object)
            delete m_data.object;
        else if (m_type == Type::array)
            delete m_data.array;

        m_type = other.m_type;
        if (m_type == Type::string)
            m_data.string = new std::string(*other.m_data.string);
        else if (m_type == Type::object)
            m_data.object = new map_type(*other.m_data.object);
        else if (m_type == Type::array)
            m_data.array = new array_type(*other.m_data.array);
        else
            m_data.bits = other.m_data.bits;

        return *this;
    }

    /// Move assignment operator.
    PropertyValue& operator=(PropertyValue&& other)
    {
        if (this == &other)
            return *this;

        if (m_type == Type::string)
            delete m_data.string;
        else if (m_type == Type::object)
            delete m_data.object;
        else if (m_type == Type::array)
            delete m_data.array;

        m_type = other.m_type;
        if (m_type == Type::string)
            m_data.string = other.m_data.string;
        else if (m_type == Type::object)
            m_data.object = other.m_data.object;
        else if (m_type == Type::array)
            m_data.array = other.m_data.array;
        else
            m_data.bits = other.m_data.bits;

        other.m_type = Type::null;
        other.m_data.bits = 0;

        return *this;
    }

    /// Destructor.
    ~PropertyValue()
    {
        if (m_type == Type::string)
            delete m_data.string;
        else if (m_type == Type::object)
            delete m_data.object;
        else if (m_type == Type::array)
            delete m_data.array;
    }

    // clang-format off
    PropertyValue() : PropertyValue(Type::null) {}
    PropertyValue(bool value) : PropertyValue(Type::boolean) { m_data.boolean = value; }
    PropertyValue(int8_t value) : PropertyValue(Type::number_signed) { m_data.number_signed = value; }
    PropertyValue(int16_t value) : PropertyValue(Type::number_signed) { m_data.number_signed = value; }
    PropertyValue(int32_t value) : PropertyValue(Type::number_signed) { m_data.number_signed = value; }
    PropertyValue(int64_t value) : PropertyValue(Type::number_signed) { m_data.number_signed = value; }
    PropertyValue(uint8_t value) : PropertyValue(Type::number_unsigned) { m_data.number_unsigned = value; }
    PropertyValue(uint16_t value) : PropertyValue(Type::number_unsigned) { m_data.number_unsigned = value; }
    PropertyValue(uint32_t value) : PropertyValue(Type::number_unsigned) { m_data.number_unsigned = value; }
    PropertyValue(uint64_t value) : PropertyValue(Type::number_unsigned) { m_data.number_unsigned = value; }
    PropertyValue(float value) : PropertyValue(Type::number_float) { m_data.number_float = value; }
    PropertyValue(double value) : PropertyValue(Type::number_float) { m_data.number_float = value; }
    PropertyValue(std::string_view value) : PropertyValue(Type::string) { *m_data.string = value; }
    PropertyValue(const std::string& value) : PropertyValue(Type::string) { *m_data.string = value; }
    PropertyValue(map_type object) : PropertyValue(Type::object) { *m_data.object = object; }
    // PropertyValue(std::span<PropertyValue> array) : PropertyValue(Type::array) { *m_data.array = array_type{array.begin(), array.end()}; }
    // PropertyValue(std::initializer_list<PropertyValue> array) : PropertyValue(Type::array) { *m_data.array = array_type(array.begin(), array.end()); }
    // clang-format on

    template<has_property_adapter T>
    PropertyValue(const T& value)
        : PropertyValue(PropertyAdapter<T>::to_property(value))
    {
    }

    static PropertyValue object() { return PropertyValue(Type::object); }

    static PropertyValue array() { return PropertyValue(Type::array); }
    static PropertyValue array(std::initializer_list<PropertyValue> values)
    {
        PropertyValue p(Type::array);
        p.m_data.array->assign(values.begin(), values.end());
        return p;
    }

    Type type() const { return m_type; }

    bool is_null() const { return m_type == Type::null; }
    bool is_boolean() const { return m_type == Type::boolean; }
    bool is_number_signed() const { return m_type == Type::number_signed; }
    bool is_number_unsigned() const { return m_type == Type::number_unsigned; }
    bool is_number_float() const { return m_type == Type::number_float; }
    bool is_number() const { return is_number_signed() || is_number_unsigned() || is_number_float(); }
    bool is_string() const { return m_type == Type::string; }
    bool is_object() const { return m_type == Type::object; }
    bool is_array() const { return m_type == Type::array; }

    bool is_array(size_t size) const { return is_array() && m_data.array->size() == size; }
    bool is_array(size_t size, Type type) const
    {
        return is_array(size)
            && std::all_of(
                   m_data.array->begin(),
                   m_data.array->end(),
                   [type](const auto& v) { return v.type() == type; }
            );
    }

    template<typename T>
    bool is_array_of_shape(size_t size) const
    {
        return is_array(size)
            && std::all_of(m_data.array->begin(), m_data.array->end(), [](const auto& v) { return v.has_shape<T>(); });
    }

    bool as_boolean() const
    {
        require_type(Type::boolean);
        return m_data.boolean;
    }

    int64_t as_number_signed() const
    {
        require_type(Type::number_signed);
        return m_data.number_signed;
    }

    uint64_t as_number_unsigned() const
    {
        require_type(Type::number_unsigned);
        return m_data.number_unsigned;
    }

    double as_number_float() const
    {
        require_type(Type::number_float);
        return m_data.number_float;
    }

    const std::string& as_string() const
    {
        require_type(Type::string);
        return *m_data.string;
    }

    const map_type& as_object() const
    {
        require_type(Type::object);
        return *m_data.object;
    }

    const array_type& as_array() const
    {
        require_type(Type::array);
        return *m_data.array;
    }

    template<typename T>
    T as() const;

    template<has_property_adapter T>
    T as() const
    {
        if (!has_shape<T>())
            KALI_THROW("Property does not have the required shape");
        return PropertyAdapter<T>::from_property(*this);
    }

    // clang-format off
    template<> bool as<bool>() const { return as_boolean(); }
    template<> int8_t as<int8_t>() const { return static_cast<int8_t>(as_number_signed()); }
    template<> int16_t as<int16_t>() const { return static_cast<int16_t>(as_number_signed()); }
    template<> int32_t as<int32_t>() const { return static_cast<int32_t>(as_number_signed()); }
    template<> int64_t as<int64_t>() const { return as_number_signed(); }
    template<> uint8_t as<uint8_t>() const { return static_cast<uint8_t>(as_number_unsigned()); }
    template<> uint16_t as<uint16_t>() const { return static_cast<uint16_t>(as_number_unsigned()); }
    template<> uint32_t as<uint32_t>() const { return static_cast<uint32_t>(as_number_unsigned()); }
    template<> uint64_t as<uint64_t>() const { return as_number_unsigned(); }
    template<> float as<float>() const { return static_cast<float>(as_number_float()); }
    template<> double as<double>() const { return as_number_float(); }
    template<> std::string as<std::string>() const { return as_string(); }
    // clang-format on

    template<typename T>
    bool has_shape() const;

    template<has_property_adapter T>
    bool has_shape() const
    {
        return PropertyAdapter<T>::has_shape(*this);
    }

    // clang-format off
    template<> bool has_shape<bool>() const { return is_boolean(); }
    template<> bool has_shape<int8_t>() const { return is_number_signed(); }
    template<> bool has_shape<int16_t>() const { return is_number_signed(); }
    template<> bool has_shape<int32_t>() const { return is_number_signed(); }
    template<> bool has_shape<int64_t>() const { return is_number_signed(); }
    template<> bool has_shape<uint8_t>() const { return is_number_unsigned(); }
    template<> bool has_shape<uint16_t>() const { return is_number_unsigned(); }
    template<> bool has_shape<uint32_t>() const { return is_number_unsigned(); }
    template<> bool has_shape<uint64_t>() const { return is_number_unsigned(); }
    template<> bool has_shape<float>() const { return is_number_float(); }
    template<> bool has_shape<double>() const { return is_number_float(); }
    template<> bool has_shape<std::string>() const { return is_string(); }
    // clang-format on

    /// Access object field by name.
    PropertyValue& operator[](std::string_view name)
    {
        require_type(Type::object);
        auto it = m_data.object->find(name);
        if (it == m_data.object->end())
            it = m_data.object->emplace(name, PropertyValue()).first;
        return it->second;
    }

    /// Access object field by name.
    const PropertyValue& operator[](std::string_view name) const
    {
        require_type(Type::object);
        auto it = m_data.object->find(name);
        return it->second;
    }

    /// Access array element by index.
    PropertyValue& operator[](size_t index)
    {
        require_type(Type::array);
        return (*m_data.array)[index];
    }

    /// Access array element by index.
    const PropertyValue& operator[](size_t index) const
    {
        require_type(Type::array);
        return (*m_data.array)[index];
    }

    std::string to_string() const
    {
        switch (m_type) {
        case Type::null:
            return "null";
        case Type::boolean:
            return m_data.boolean ? "true" : "false";
        case Type::number_signed:
            return std::to_string(m_data.number_signed);
        case Type::number_unsigned:
            return std::to_string(m_data.number_unsigned);
        case Type::number_float:
            return std::to_string(m_data.number_float);
        case Type::string:
            return "\"" + *m_data.string + "\"";
        case Type::object: {
            std::string s = "{";
            for (auto it = m_data.object->begin(); it != m_data.object->end(); ++it) {
                if (it != m_data.object->begin())
                    s += ", ";
                s += "\"" + it->first + "\": ";
                s += it->second.to_string();
            }
            s += "}";
            return s;
        }
        case Type::array: {
            std::string s = "[";
            for (size_t i = 0; i < m_data.array->size(); ++i) {
                if (i > 0)
                    s += ", ";
                s += (*m_data.array)[i].to_string();
            }
            s += "]";
            return s;
        }
        }
        KALI_THROW("Unknown property type");
    }

private:
    void require_type(Type type) const
    {
        if (m_type != type)
            KALI_THROW("Property is not of the required type");
    }

    Type m_type;
    union {
        bool boolean;
        int64_t number_signed;
        uint64_t number_unsigned;
        double number_float;
        std::string* string;
        map_type* object;
        array_type* array;
        uint64_t bits;
    } m_data;
};


template<>
struct PropertyAdapter<float3> {
    static bool has_shape(const PropertyValue& p) { return p.is_array(3, PropertyValue::Type::number_float); }
    static PropertyValue to_property(const float3& v) { return PropertyValue::array({v.x, v.y, v.z}); }
    static float3 from_property(const PropertyValue& p)
    {
        return float3{p[0].as<float>(), p[1].as<float>(), p[2].as<float>()};
    }
};

template<>
struct PropertyAdapter<float4> {
    static bool has_shape(const PropertyValue& p) { return p.is_array(4, PropertyValue::Type::number_float); }
    static PropertyValue to_property(const float4& v) { return PropertyValue::array({v.x, v.y, v.z, v.w}); }
    static float4 from_property(const PropertyValue& p)
    {
        return float4{p[0].as<float>(), p[1].as<float>(), p[2].as<float>(), p[3].as<float>()};
    }
};

} // namespace kali
