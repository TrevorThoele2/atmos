
#include "ModulatorValue.h"

#include <Inscription/Scribe.h>

namespace Atmos
{
    namespace Modulator
    {
        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Value)
        {
            scribe(type);
            switch (type)
            {
            case Type::INT:
                scribe(i);
                break;
            case Type::FLOAT:
                scribe(f);
                break;
            }
        }

        Value::Value() : i(std::int64_t(0)), type(Type::NONE)
        {}

        Value::Value(std::int64_t i) : i(i), type(Type::INT)
        {}

        Value::Value(float f) : f(f), type(Type::FLOAT)
        {}

        Value::~Value()
        {}

        bool Value::operator==(const Value &arg) const
        {
            if (type != arg.type)
                return false;

            switch (type)
            {
            case Type::INT:
                return i == arg.i;
            case Type::FLOAT:
                return f == arg.f;
            }

            return true;
        }

        bool Value::operator!=(const Value &arg) const
        {
            return !(*this == arg);
        }

        void Value::Set()
        {
            type = Type::NONE;
            i = 0;
        }

        void Value::Set(std::int64_t set)
        {
            type = Type::INT;
            i = set;
        }

        void Value::Set(float set)
        {
            type = Type::FLOAT;
            f = set;
        }

        bool Value::IsNone() const
        {
            return type == Type::NONE;
        }

        bool Value::IsInt() const
        {
            return type == Type::INT;
        }

        bool Value::IsFloat() const
        {
            return type == Type::FLOAT;
        }

        Value::Type Value::GetType() const
        {
            return type;
        }

        std::int64_t Value::AsInt() const
        {
            return i;
        }

        float Value::AsFloat() const
        {
            return f;
        }
    }
}