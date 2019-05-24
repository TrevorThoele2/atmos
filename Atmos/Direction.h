
#pragma once

#include <iterator>
#include "Serialization.h"
#include <Chroma/Enum.h>

namespace Atmos
{
    class Direction
    {
    public:
        enum ValueT : unsigned char
        {
            LEFT,
            UP,
            RIGHT,
            DOWN,
            Z_UP,
            Z_DOWN
        };

        enum class RelativeValueT : unsigned char
        {
            SAME,
            LEFT,
            RIGHT,
            OPPOSED
        };
    private:
        INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        ValueT value;

        template<ValueT val>
        static constexpr std::underlying_type<ValueT>::type CastFromValueT();
    public:
        Direction(ValueT value = ValueT::UP);
        Direction(const Direction &arg) = default;
        Direction& operator=(const Direction &arg) = default;
        Direction& operator=(ValueT arg);
        bool operator==(const Direction &arg) const;
        bool operator!=(const Direction &arg) const;
        explicit operator ValueT() const;

        void Set(ValueT set);
        ValueT Get() const;

        void MoveInRelativeDirection(RelativeValueT move);
        RelativeValueT GetRelativeDirection(ValueT value) const;
        RelativeValueT GetRelativeDirection(const Direction &direction) const;

        static ValueT FromUnderlyingType(std::underlying_type<ValueT>::type from);
    };

    template<Direction::ValueT val>
    constexpr std::underlying_type<Direction::ValueT>::type Direction::CastFromValueT()
    {
        return static_cast<std::underlying_type<Direction::ValueT>::type>(val);
    }

    typedef ::Chroma::EnumIterationTraits<Direction::ValueT, Direction::ValueT::LEFT, Direction::ValueT::DOWN> DirectionIterationTraits;
}