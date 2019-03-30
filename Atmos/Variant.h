#pragma once

#include <stdint.h>

#include "String.h"
#include "StringUtility.h"

#include <Chroma/Variant.h>

namespace Atmos
{
    typedef ::Chroma::Variant<
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
        String> VariantT;

    String ToString(const VariantT& arg);
}