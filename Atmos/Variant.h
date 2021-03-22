#pragma once

#include <variant>
#include <Inscription/VariantScribe.h>

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
        std::string>;
}