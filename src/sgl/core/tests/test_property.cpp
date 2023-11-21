#include "testing.h"
#include "kali/core/property.h"
#include "kali/math/vector.h"

using namespace kali;

TEST_SUITE_BEGIN("property");

TEST_CASE("basics")
{
    PropertyValue value{true};
    PropertyValue value2{(int64_t)1};
    PropertyValue value3{1};
    PropertyValue value4{1.0};
    PropertyValue value5{1.f};
    PropertyValue value6{{true, true, false}};
    PropertyValue value7{{{"a", true}, {"b", false}}};

    auto obj = PropertyValue::object();
    obj["a"] = true;
    obj["b"] = false;
    // obj["c"] = float3(1.f, 2.f, 3.f);
    obj["c"] = PropertyValue::array({1.f, 2.f, 3.f, 4.f});

    fmt::print("{}", obj.to_string());

    bool a = obj["a"].as<bool>();
    bool b = obj["b"].as<bool>();
    float4 c = obj["c"].as<float4>();

    if (obj["c"].has_shape<float3>()) {
        float3 c2 = obj["c"].as<float3>();
        fmt::print("{}", c2);
    }

    auto arr = PropertyValue::array();
    // arr.push_back(true);
}

TEST_SUITE_END();
