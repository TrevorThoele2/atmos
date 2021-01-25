#pragma once

#include <cstdint>
#include "String.h"

namespace Atmos
{
    template<class T>
    struct PrimitiveTraits;

    template<>
    struct PrimitiveTraits<bool>
    {
        static String TypeName() { return "bool"; }
    };

    template<>
    struct PrimitiveTraits<std::int8_t>
    {
        static String TypeName() { return "std::int8_t"; }
    };

    template<>
    struct PrimitiveTraits<std::int16_t>
    {
        static String TypeName() { return "std::int16_t"; }
    };

    template<>
    struct PrimitiveTraits<std::int32_t>
    {
        static String TypeName() { return "std::int32_t"; }
    };

    template<>
    struct PrimitiveTraits<std::int64_t>
    {
        static String TypeName() { return "std::int64_t"; }
    };

    template<>
    struct PrimitiveTraits<std::uint8_t>
    {
        static String TypeName() { return "std::uint8_t"; }
    };

    template<>
    struct PrimitiveTraits<std::uint16_t>
    {
        static String TypeName() { return "std::uint16_t"; }
    };

    template<>
    struct PrimitiveTraits<std::uint32_t>
    {
        static String TypeName() { return "std::uint32_t"; }
    };

    template<>
    struct PrimitiveTraits<std::uint64_t>
    {
        static String TypeName() { return "std::uint64_t"; }
    };

    template<>
    struct PrimitiveTraits<float>
    {
        static String TypeName() { return "float"; }
    };

    template<>
    struct PrimitiveTraits<double>
    {
        static String TypeName() { return "double"; }
    };
}