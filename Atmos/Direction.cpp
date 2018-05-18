
#include "Direction.h"

#include <Inscription\Scribe.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Direction)
    {
        scribe(value);
    }

    Direction::Direction(ValueT value) : value(value)
    {}

    Direction& Direction::operator=(ValueT arg)
    {
        value = arg;
        return *this;
    }

    bool Direction::operator==(const Direction &arg) const
    {
        return value == arg.value;
    }

    bool Direction::operator!=(const Direction &arg) const
    {
        return !(*this == arg);
    }

    Direction::operator ValueT() const
    {
        return value;
    }

    void Direction::Set(ValueT set)
    {
        value = set;
    }

    Direction::ValueT Direction::Get() const
    {
        return value;
    }

    void Direction::MoveInRelativeDirection(RelativeValueT move)
    {
        switch (move)
        {
        case RelativeValueT::SAME:
            return;
        case RelativeValueT::LEFT:
            switch (value)
            {
            case ValueT::UP:
                value = ValueT::LEFT;
                return;
            case ValueT::DOWN:
                value = ValueT::RIGHT;
                return;
            case ValueT::LEFT:
                value = ValueT::DOWN;
                return;
            case ValueT::RIGHT:
                value = ValueT::UP;
                return;
            }

        case RelativeValueT::RIGHT:
            switch (value)
            {
            case ValueT::UP:
                value = ValueT::RIGHT;
                return;
            case ValueT::DOWN:
                value = ValueT::LEFT;
                return;
            case ValueT::LEFT:
                value = ValueT::UP;
                return;
            case ValueT::RIGHT:
                value = ValueT::DOWN;
                return;
            }

        case RelativeValueT::OPPOSED:
            switch (value)
            {
            case ValueT::UP:
                value = ValueT::DOWN;
                return;
            case ValueT::DOWN:
                value = ValueT::UP;
                return;
            case ValueT::LEFT:
                value = ValueT::RIGHT;
                return;
            case ValueT::RIGHT:
                value = ValueT::LEFT;
                return;
            }
        }
    }

    Direction::RelativeValueT Direction::GetRelativeDirection(ValueT value) const
    {
        switch (this->value)
        {
        case ValueT::UP:
            switch (value)
            {
            case ValueT::UP:
                return RelativeValueT::SAME;
            case ValueT::DOWN:
                return RelativeValueT::OPPOSED;
            case ValueT::LEFT:
                return RelativeValueT::LEFT;
            case ValueT::RIGHT:
                return RelativeValueT::RIGHT;
            }
        case ValueT::DOWN:
            switch (value)
            {
            case ValueT::UP:
                return RelativeValueT::OPPOSED;
            case ValueT::DOWN:
                return RelativeValueT::SAME;
            case ValueT::LEFT:
                return RelativeValueT::RIGHT;
            case ValueT::RIGHT:
                return RelativeValueT::LEFT;
            }
        case ValueT::LEFT:
            switch (value)
            {
            case ValueT::UP:
                return RelativeValueT::RIGHT;
            case ValueT::DOWN:
                return RelativeValueT::LEFT;
            case ValueT::LEFT:
                return RelativeValueT::SAME;
            case ValueT::RIGHT:
                return RelativeValueT::OPPOSED;
            }
        case ValueT::RIGHT:
            switch (value)
            {
            case ValueT::UP:
                return RelativeValueT::LEFT;
            case ValueT::DOWN:
                return RelativeValueT::RIGHT;
            case ValueT::LEFT:
                return RelativeValueT::OPPOSED;
            case ValueT::RIGHT:
                return RelativeValueT::SAME;
            }
        }

        return RelativeValueT::SAME;
    }

    Direction::RelativeValueT Direction::GetRelativeDirection(const Direction &direction) const
    {
        return GetRelativeDirection(direction.Get());
    }

    Direction::ValueT Direction::FromUnderlyingType(std::underlying_type<ValueT>::type from)
    {
#define CAST_PIECE(ValueTT) case CastFromValueT<ValueT::ValueTT>(): return ValueT::ValueTT;
        switch (from)
        {
            CAST_PIECE(UP)
            CAST_PIECE(DOWN)
            CAST_PIECE(LEFT)
            CAST_PIECE(RIGHT)
        }

        return Direction::ValueT::UP;
#undef CAST_PIECE
    }
}