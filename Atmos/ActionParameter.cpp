
#include "ActionParameter.h"

namespace Atmos
{
    namespace Act
    {
        ParameterTypeError::ParameterTypeError() : runtime_error("A parameter has been attempted to be extracted as a different type than it is.")
        {}

        Parameter::Parameter()
        {}

        Parameter::Parameter(const Variant &variant) : val(variant)
        {}

        Parameter::Parameter(Variant &&variant) : val(std::move(variant))
        {}

        Parameter::Parameter(const Parameter &arg) : val(arg.val)
        {}

        Parameter::Parameter(Parameter &&arg) : val(std::move(arg.val))
        {}

        Parameter& Parameter::operator=(const Variant &arg)
        {
            val = arg;
            return *this;
        }

        Parameter& Parameter::operator=(Variant &&arg)
        {
            val = std::move(arg);
            return *this;
        }

        Parameter& Parameter::operator=(const Parameter &arg)
        {
            val = arg.val;
            return *this;
        }

        Parameter& Parameter::operator=(Parameter &&arg)
        {
            val = std::move(arg.val);
            return *this;
        }

        Variant* Parameter::operator->()
        {
            return &val;
        }

        const Variant* Parameter::operator->() const
        {
            return &val;
        }

        Variant& Parameter::operator*()
        {
            return val;
        }

        const Variant& Parameter::operator*() const
        {
            return val;
        }

        Variant& Parameter::Get()
        {
            return val;
        }

        const Variant& Parameter::Get() const
        {
            return val;
        }

        Variant::Type Parameter::GetType() const
        {
            return val.GetType();
        }
    }
}