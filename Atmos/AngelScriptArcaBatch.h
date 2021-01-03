#pragma once

#include "AngelScriptObjectRegistration.h"
#include "AngelScriptObjectManagement.h"
#include "AngelScriptPrimitive.h"
#include <Arca/RelicBatch.h>
#include <Arca/Reliquary.h>

namespace Atmos::Scripting::Angel
{
    class GenericArcaBatch
    {};

    template<>
    struct Registration<GenericArcaBatch>
    {
        using Type = GenericArcaBatch;

        static inline const String name = "Batch<class T>";
        static inline const String containingNamespace = "Arca";
        static inline const String documentation = "Needs to be used with explicit specializations.";
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<class T>
    struct Registration<Arca::Batch<T>>
    {
        using Type = Arca::Batch<T>;

        static inline const String name = "Batch<" + CreateName({ Registration<T>::containingNamespace }, Registration<T>::name) + ">";
        static inline const String containingNamespace = "Arca";
        static inline const String documentation = "Needs to be used with explicit specializations.";
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static std::vector<Arca::Index<T>> Values(Type, Arca::Reliquary* reliquary);
    };
    
    template<class T>
    void Registration<Arca::Batch<T>>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(containingNamespace, name)
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .ConstMethod(
                &Management::template Method<&Values, &UserData::RequiredReliquaryFrom>,
                CreateName({ Registration<T>::containingNamespace }, Registration<T>::name + "[]@"),
                "Values",
                {})
            .Actualize(engine, documentationManager);
    }

    template<class T>
    std::vector<Arca::Index<T>> Registration<Arca::Batch<T>>::Values(Type, Arca::Reliquary* reliquary)
    {
        auto batch = reliquary->Batch<T>();

        std::vector<Arca::Index<T>> returnValue;
        returnValue.reserve(batch.Size());

        for (auto element = batch.begin(); element != batch.end(); ++element)
            returnValue.push_back({ element.ID(), reliquary });

        return returnValue;
    }
}