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

        static inline const String name = "Traits<class T>";
        static inline const String containingNamespace = "Arca";
        static inline const String documentation = "Traits exist for every Arca relic, shard, command and signal.";
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

        static inline const String name = "Traits<" + CreateName({ Registration<T>::containingNamespace }, Registration<T>::name) + ">";
        static inline const String containingNamespace = "Arca";
        static inline const String documentation = "Traits exist for every Arca relic, shard, command and signal.";
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<ArcaTraits<T>>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<class T>
    void Registration<ArcaTraits<T>>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(containingNamespace, name)
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .ConstMethod(&Management::template Method<&Type::Type>, "Arca::Type", "Type", {})
            .Actualize(engine, documentationManager);
    }
}