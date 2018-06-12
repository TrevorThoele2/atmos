
#include "ModulatorValue.h"

#include <Inscription/Scribe.h>

namespace Atmos
{
    namespace Modulator
    {
        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Value)
        {
            if (scribe.IsOutput())
            {
                scribe.Save(variant.GetTypeAsID());
                scribe.Save(variant.GetMaster());
            }
            else // INPUT
            {
                VariantT::ID id;
                scribe.Load(id);
                VariantT::MasterType master;
                scribe.Load(master);
                variant.SetMaster(master, id);
            }
        }

        Value::Value() : variant(std::int64_t(0))
        {}

        Value::Value(std::int64_t i) : variant(i)
        {}

        Value::Value(float f) : variant(f)
        {}

        Value::~Value()
        {}

        bool Value::operator==(const Value &arg) const
        {
            return variant == arg.variant;
        }

        bool Value::operator!=(const Value &arg) const
        {
            return !(*this == arg);
        }

        void Value::Set()
        {
            variant.Set(std::int64_t(0));
        }

        void Value::Set(std::int64_t set)
        {
            variant.Set(std::move(set));
        }

        void Value::Set(float set)
        {
            variant.Set(std::move(set));
        }

        void Value::Convert(Type type)
        {
            if (type == GetType())
                return;

            if (type == Type::INT)
                variant.Set(static_cast<std::int64_t>(variant.Get<float>()));
            else if (type == Type::FLOAT)
                variant.Set(static_cast<float>(variant.Get<std::int64_t>()));
        }

        bool Value::IsNone() const
        {
            return variant.IsInhabited();
        }

        bool Value::IsInt() const
        {
            return variant.Is<std::int64_t>();
        }

        bool Value::IsFloat() const
        {
            return variant.Is<float>();
        }

        Value::Type Value::GetType() const
        {
            if (IsInt())
                return Type::INT;
            else if (IsFloat())
                return Type::FLOAT;
            
            return Type::NONE;
        }

        std::int64_t Value::AsInt() const
        {
            if (!variant.IsInhabited() || !IsInt())
                return 0;

            return variant.Get<std::int64_t>();
        }

        float Value::AsFloat() const
        {
            if (!variant.IsInhabited() || !IsFloat())
                return 0;

            return variant.Get<float>();
        }
    }
}