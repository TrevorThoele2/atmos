#pragma once

#include "Range.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

#include "AngelScriptObjectRegistration.h"
#include "AngelScriptGlobalManagement.h"
#include <angelscript.h>

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<class T>
    struct Registration<Range<T>>
    {
        using Type = Range<T>;
        using Management = ObjectManagement<Type>;

        static String Name() { return TypeString() + "Range"; }
        static String ContainingNamespace() { return "Atmos"; }
        static String Documentation() { return "Represents a range of values."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static String TypeString();
    };

    template<class T>
    void Registration<Range<T>>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        const auto typeString = Registration<T>::Name();

        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .Constructor(
                &Management::template GenerateValue<&PullFromParameter<0, T>, &PullFromParameter<1, T>>,
                { typeString + " min", typeString + " max" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .template Property<&Type::min>(typeString, "min")
            .template Property<&Type::max>(typeString, "max")
            .Actualize(engine, documentationManager);
    }

    template<class T>
    String Registration<Range<T>>::TypeString()
    {
        const auto baseName = Registration<T>::Name();
        return Chroma::ToUppercase(baseName.substr(0, 1)) + baseName.substr(1, String::npos);
    }
}