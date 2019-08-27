
#include "Direction.h"

#include <Inscription\Scribe.h>

namespace Atmos
{
    Direction::Direction(Value value) : value(value)
    {}

    Direction& Direction::operator=(Value arg)
    {
        value = arg;
        return *this;
    }

    bool Direction::operator==(const Direction& arg) const
    {
        return value == arg.value;
    }

    bool Direction::operator!=(const Direction& arg) const
    {
        return !(*this == arg);
    }

    Direction::operator Value() const
    {
        return value;
    }

    void Direction::Set(Value set)
    {
        value = set;
    }

    Direction::Value Direction::Get() const
    {
        return value;
    }

    void Direction::MoveInRelativeDirection(RelativeValue move)
    {
        switch (move)
        {
        case RelativeValue::Same:
            return;
        case RelativeValue::Left:
            switch (value)
            {
            case Value::Up:
                value = Value::Left;
                return;
            case Value::Down:
                value = Value::Right;
                return;
            case Value::Left:
                value = Value::Down;
                return;
            case Value::Right:
                value = Value::Up;
                return;
            }

        case RelativeValue::Right:
            switch (value)
            {
            case Value::Up:
                value = Value::Right;
                return;
            case Value::Down:
                value = Value::Left;
                return;
            case Value::Left:
                value = Value::Up;
                return;
            case Value::Right:
                value = Value::Down;
                return;
            }

        case RelativeValue::Opposed:
            switch (value)
            {
            case Value::Up:
                value = Value::Down;
                return;
            case Value::Down:
                value = Value::Up;
                return;
            case Value::Left:
                value = Value::Right;
                return;
            case Value::Right:
                value = Value::Left;
                return;
            }
        }
    }

    Direction::RelativeValue Direction::GetRelativeDirection(Value value) const
    {
        switch (this->value)
        {
        case Value::Up:
            switch (value)
            {
            case Value::Up:
                return RelativeValue::Same;
            case Value::Down:
                return RelativeValue::Opposed;
            case Value::Left:
                return RelativeValue::Left;
            case Value::Right:
                return RelativeValue::Right;
            }
        case Value::Down:
            switch (value)
            {
            case Value::Up:
                return RelativeValue::Opposed;
            case Value::Down:
                return RelativeValue::Same;
            case Value::Left:
                return RelativeValue::Right;
            case Value::Right:
                return RelativeValue::Left;
            }
        case Value::Left:
            switch (value)
            {
            case Value::Up:
                return RelativeValue::Right;
            case Value::Down:
                return RelativeValue::Left;
            case Value::Left:
                return RelativeValue::Same;
            case Value::Right:
                return RelativeValue::Opposed;
            }
        case Value::Right:
            switch (value)
            {
            case Value::Up:
                return RelativeValue::Left;
            case Value::Down:
                return RelativeValue::Right;
            case Value::Left:
                return RelativeValue::Opposed;
            case Value::Right:
                return RelativeValue::Same;
            }
        }

        return RelativeValue::Same;
    }

    Direction::RelativeValue Direction::GetRelativeDirection(const Direction& direction) const
    {
        return GetRelativeDirection(direction.Get());
    }

    Direction::Value Direction::FromUnderlyingType(std::underlying_type<Value>::type from)
    {
#define CAST_PIECE(value) case CastFromValue<Value::value>(): return Value::value;
        switch (from)
        {
            CAST_PIECE(Up)
            CAST_PIECE(Down)
            CAST_PIECE(Left)
            CAST_PIECE(Right)
        }

        return Direction::Value::Up;
#undef CAST_PIECE
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Direction, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.value);
    }
}