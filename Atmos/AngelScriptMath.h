#pragma once

#include "StringUtility.h"

#include "AngelScriptGlobalManagement.h"
#include "AngelScriptObjectType.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    class Math
    {};

    template<>
    struct Registration<Math>
    {
        using Type = Math;
        using Management = GlobalManagement;

        static String Name() { return "Math"; }
        static String ContainingNamespace() { return "Atmos"; }
        static String Documentation() { return ""; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        template<class Value>
        [[nodiscard]] static Value Floor(Value value);
        template<class Value>
        [[nodiscard]] static Value Ceiling(Value value);
        template<class Value>
        [[nodiscard]] static Value Round(Value value);
        template<class Value, class As>
        [[nodiscard]] static Value AbsoluteValue(Value value);
        template<class Value>
        [[nodiscard]] static Value Sin(Value value);
        template<class Value>
        [[nodiscard]] static Value Cos(Value value);
        template<class Value>
        [[nodiscard]] static Value Tan(Value value);
        template<class Value>
        [[nodiscard]] static Value Asin(Value value);
        template<class Value>
        [[nodiscard]] static Value Acos(Value value);
        template<class Value>
        [[nodiscard]] static Value Atan(Value value);
        [[nodiscard]] static double Pi();
    };

    template<class Value>
    Value Registration<Math>::Floor(Value value)
    {
        return std::floor(value);
    }

    template<class Value>
    Value Registration<Math>::Ceiling(Value value)
    {
        return std::ceil(value);
    }

    template<class Value>
    Value Registration<Math>::Round(Value value)
    {
        return std::round(value);
    }

    template<class Value, class As>
    Value Registration<Math>::AbsoluteValue(Value value)
    {
        const auto castedValue = static_cast<As>(value);
        const auto absoluteValue = std::abs(castedValue);
        return static_cast<Value>(absoluteValue);
    }

    template<class Value>
    Value Registration<Math>::Sin(Value value)
    {
        return std::sin(value);
    }

    template<class Value>
    Value Registration<Math>::Cos(Value value)
    {
        return std::cos(value);
    }

    template<class Value>
    Value Registration<Math>::Tan(Value value)
    {
        return std::tan(value);
    }

    template<class Value>
    Value Registration<Math>::Asin(Value value)
    {
        return std::asin(value);
    }

    template<class Value>
    Value Registration<Math>::Acos(Value value)
    {
        return std::acos(value);
    }

    template<class Value>
    Value Registration<Math>::Atan(Value value)
    {
        return std::atan(value);
    }
}