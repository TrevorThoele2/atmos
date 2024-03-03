
#pragma once

#include <iterator>
#include "Serialization.h"
#include <Chroma/Enum.h>

namespace Atmos
{
    class Direction
    {
    public:
        enum Value : unsigned char
        {
            LEFT,
            UP,
            RIGHT,
            DOWN,
            Z_UP,
            Z_DOWN
        };

        enum class RelativeValue : unsigned char
        {
            SAME,
            LEFT,
            RIGHT,
            OPPOSED
        };
    public:
        Direction(Value value = Value::UP);
        Direction(const Direction& arg) = default;

        Direction& operator=(const Direction& arg) = default;
        Direction& operator=(Value arg);

        bool operator==(const Direction& arg) const;
        bool operator!=(const Direction& arg) const;

        explicit operator Value() const;

        void Set(Value set);
        Value Get() const;

        void MoveInRelativeDirection(RelativeValue move);
        RelativeValue GetRelativeDirection(Value value) const;
        RelativeValue GetRelativeDirection(const Direction& direction) const;

        static Value FromUnderlyingType(std::underlying_type<Value>::type from);
    private:
        Value value;

        template<Value value>
        static constexpr std::underlying_type<Value>::type CastFromValue();
    private:
        INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };

    template<Direction::Value value>
    constexpr std::underlying_type<Direction::Value>::type Direction::CastFromValue()
    {
        return static_cast<std::underlying_type<Direction::Value>::type>(value);
    }

    typedef ::Chroma::EnumIterationTraits<Direction::Value, Direction::Value::LEFT, Direction::Value::DOWN> DirectionIterationTraits;
}