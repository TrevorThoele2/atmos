#pragma once

#include <cstdint>
#include <variant>

#include "String.h"

namespace Atmos
{
    using Variant = std::variant<
        bool,
        std::int8_t,
        std::int16_t,
        std::int32_t,
        std::int64_t,
        std::uint8_t,
        std::uint16_t,
        std::uint32_t,
        std::uint64_t,
        float,
        double,
        String>;

    String ToString(const Variant& arg);
}