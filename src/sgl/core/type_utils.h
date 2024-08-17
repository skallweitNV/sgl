// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "sgl/core/error.h"

#include <utility>

namespace sgl {

template<typename To, typename From>
To checked_cast(From value)
{
    SGL_ASSERT(dynamic_cast<To>(value) == static_cast<To>(value));
    return static_cast<To>(value);
}

template<typename R, typename T>
R narrow_cast(T value)
{
    if (!std::in_range<R>(value))
        SGL_THROW("narrow_cast failed");
    return static_cast<R>(value);
}

} // namespace sgl
