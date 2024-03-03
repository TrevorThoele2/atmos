#pragma once

#include <numeric>

#include "AngelScriptGlobalRegistration.h"
#include "AngelScriptGlobalManagement.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    template<class T>
    struct Registration<std::numeric_limits<T>>
    {
        using Type = std::numeric_limits<T>;
        using Management = GlobalManagement;

        static String ContainingNamespace() { return TypeString() + "Limits"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static String TypeString();

        [[nodiscard]] static T Min();
        [[nodiscard]] static T Lowest();
        [[nodiscard]] static T Max();
    };

    template<class T>
    void Registration<std::numeric_limits<T>>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        const auto typeString = Registration<T>::Name();

        GlobalRegistration(ContainingNamespace())
            .Function(&Management::template Function<&Min>, typeString, "Min", {})
            .Function(&Management::template Function<&Lowest>, typeString, "Lowest", {})
            .Function(&Management::template Function<&Max>, typeString, "Max", {})
            .Actualize(engine, documentationManager);
    }

    template<class T>
    String Registration<std::numeric_limits<T>>::TypeString()
    {
        const auto baseName = Registration<T>::Name();
        return Chroma::ToUppercase(baseName.substr(0, 1)) + baseName.substr(1, String::npos);
    }

    template<class T>
    T Registration<std::numeric_limits<T>>::Min()
    {
        return std::numeric_limits<T>::min();
    }

    template<class T>
    T Registration<std::numeric_limits<T>>::Lowest()
    {
        return std::numeric_limits<T>::lowest();
    }

    template<class T>
    T Registration<std::numeric_limits<T>>::Max()
    {
        return std::numeric_limits<T>::max();
    }
}