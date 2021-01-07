#pragma once

#include <Arca/TypeFor.h>

#include "AngelScriptObjectRegistration.h"
#include "AngelScriptObjectManagement.h"

namespace Atmos::Scripting::Angel
{
    class GenericArcaTraits
    {};

    template<>
    struct Registration<GenericArcaTraits>
    {
        using Type = GenericArcaTraits;

        static String Name() { return "Traits<class T>"; }
        static String ContainingNamespace() { return "Arca"; }
        static String Documentation() { return "Traits exist for every Arca relic, shard, command and signal."; }
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<class T>
    struct ArcaTraits
    {
        [[nodiscard]] Arca::Type Type() const;
    };

    template<class T>
    Arca::Type ArcaTraits<T>::Type() const
    {
        return Arca::TypeFor<T>();
    }

    template<class T>
    struct Registration<ArcaTraits<T>>
    {
        using Type = ArcaTraits<T>;

        static String Name() { return "Traits<" + CreateName({ Registration<T>::ContainingNamespace() }, Registration<T>::Name()) + ">"; }
        static String ContainingNamespace() { return "Arca"; }
        static String Documentation() { return "Traits exist for every Arca relic, shard, command and signal."; }
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<ArcaTraits<T>>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<class T>
    void Registration<ArcaTraits<T>>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .ConstMethod(&Management::template Method<&Type::Type>, "Arca::Type", "Type", {})
            .Actualize(engine, documentationManager);
    }
}