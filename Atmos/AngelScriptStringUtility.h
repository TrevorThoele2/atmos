#pragma once

#include "StringUtility.h"

#include "AngelScriptGlobalManagement.h"
#include "AngelScriptObjectType.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    class StringUtility
    {
    public:
        template<class T>
        static String ToString(T value);
        template<class T>
        static T FromString(String value);
    };

    template<class T>
    String StringUtility::ToString(T value)
    {
        return Chroma::ToString(value);
    }

    template<class T>
    T StringUtility::FromString(String value)
    {
        return Chroma::FromString<T>(value);
    }

    template<>
    struct Registration<StringUtility>
    {
        using Type = StringUtility;
        using Management = GlobalManagement;

        static String Name() { return "StringUtility"; }
        static String ContainingNamespace() { return "Atmos"; }
        static String Documentation() { return ""; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static String Join(String joiner, std::vector<String> tokens);
    };
}