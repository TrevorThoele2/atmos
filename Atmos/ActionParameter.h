#pragma once

#include <cstdint>
#include <string>
#include <memory>

#include "ActionID.h"
#include "Variant.h"
#include "Optional.h"

namespace Atmos
{
    namespace Act
    {
        class ParameterTypeError : public std::runtime_error
        {
        public:
            ParameterTypeError();
        };

        class Parameter
        {
        private:
            Variant val;
        public:
            Parameter();
            Parameter(const Variant &variant);
            Parameter(Variant &&variant);
            Parameter(const Parameter &arg);
            Parameter(Parameter &&arg);
            Parameter& operator=(const Variant &arg);
            Parameter& operator=(Variant &&arg);
            Parameter& operator=(const Parameter &arg);
            Parameter& operator=(Parameter &&arg);

            Variant* operator->();
            const Variant* operator->() const;
            Variant& operator*();
            const Variant& operator*() const;
            Variant& Get();
            const Variant& Get() const;

            Variant::Type GetType() const;
            template<Variant::Type t>
            typename const VariantTraits<t>::T GetAs() const;
            template<class T>
            T GetAs() const;
        };

        template<Variant::Type t>
        typename const VariantTraits<t>::T Parameter::GetAs() const
        {
            if(val.Is<t>())
                throw ParameterTypeError();

            return val.As<t>();
        }

        template<class T>
        T Parameter::GetAs() const
        {
            if (!val.Is<T>())
                throw ParameterTypeError();

            return val.As<T>();
        }
    }
}