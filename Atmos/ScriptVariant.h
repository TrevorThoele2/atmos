#pragma once

#include <stdint.h>
#include <variant>

#include "StringUtility.h"

namespace Atmos::Script
{
    using Variant = std::variant<
        bool,
        std::int8_t,
        std::int16_t,
        std::int32_t,
        std::int64_t,
        std::uint8_t,
        std::uint16_t,
        uint32_t,
        std::uint64_t,
        float,
        double
    >;
}

namespace Atmos
{
    String ToString(const Script::Variant& arg);
}