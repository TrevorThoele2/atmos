#pragma once

#include <stdint.h>

#include "StringUtility.h"

#include <Chroma/Variant.h>

namespace Atmos
{
    namespace Scripting
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
            double> VariantT;
    }

    String ToString(const Scripting::VariantT &arg);
}