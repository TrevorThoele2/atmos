
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
        case RelativeValue::SAME:
            return;
        case RelativeValue::LEFT:
            switch (value)
            {
            case Value::UP:
                value = Value::LEFT;
                return;
            case Value::DOWN:
                value = Value::RIGHT;
                return;
            case Value::LEFT:
                value = Value::DOWN;
                return;
            case Value::RIGHT:
                value = Value::UP;
                return;
            }

        case RelativeValue::RIGHT:
            switch (value)
            {
            case Value::UP:
                value = Value::RIGHT;
                return;
            case Value::DOWN:
                value = Value::LEFT;
                return;
            case Value::LEFT:
                value = Value::UP;
                return;
            case Value::RIGHT:
                value = Value::DOWN;
                return;
            }

        case RelativeValue::OPPOSED:
            switch (value)
            {
            case Value::UP:
                value = Value::DOWN;
                return;
            case Value::DOWN:
                value = Value::UP;
                return;
            case Value::LEFT:
                value = Value::RIGHT;
                return;
            case Value::RIGHT:
                value = Value::LEFT;
                return;
            }
        }
    }

    Direction::RelativeValue Direction::GetRelativeDirection(Value value) const
    {
        switch (this->value)
        {
        case Value::UP:
            switch (value)
            {
            case Value::UP:
                return RelativeValue::SAME;
            case Value::DOWN:
                return RelativeValue::OPPOSED;
            case Value::LEFT:
                return RelativeValue::LEFT;
            case Value::RIGHT:
                return RelativeValue::RIGHT;
            }
        case Value::DOWN:
            switch (value)
            {
            case Value::UP:
                return RelativeValue::OPPOSED;
            case Value::DOWN:
                return RelativeValue::SAME;
            case Value::LEFT:
                return RelativeValue::RIGHT;
            case Value::RIGHT:
                return RelativeValue::LEFT;
            }
        case Value::LEFT:
            switch (value)
            {
            case Value::UP:
                return RelativeValue::RIGHT;
            case Value::DOWN:
                return RelativeValue::LEFT;
            case Value::LEFT:
                return RelativeValue::SAME;
            case Value::RIGHT:
                return RelativeValue::OPPOSED;
            }
        case Value::RIGHT:
            switch (value)
            {
            case Value::UP:
                return RelativeValue::LEFT;
            case Value::DOWN:
                return RelativeValue::RIGHT;
            case Value::LEFT:
                return RelativeValue::OPPOSED;
            case Value::RIGHT:
                return RelativeValue::SAME;
            }
        }

        return RelativeValue::SAME;
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
            CAST_PIECE(UP)
            CAST_PIECE(DOWN)
            CAST_PIECE(LEFT)
            CAST_PIECE(RIGHT)
        }

        return Direction::Value::UP;
#undef CAST_PIECE
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Direction, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        archive(object.value);
    }
}