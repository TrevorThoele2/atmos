#pragma once

#include "FindAssetByName.h"

#include "AngelScriptObjectRegistration.h"
#include "AngelScriptFactory.h"
#include "AngelScriptCommand.h"

#include <Chroma/Identity.h>

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    class GenericAssetFindByName
    {};

    template<>
    struct Registration<GenericAssetFindByName>
    {
        using Type = GenericAssetFindByName;

        static inline const String name = "FindByName<class T>";
        static inline const String containingNamespace = "Atmos::Asset";
        static inline const String documentation = "This is a command. Needs to be used with explicit specializations.";
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<class T>
    struct Registration<Asset::FindByName<T>>
    {
        using Type = Asset::FindByName<T>;

        static inline const String name = "FindByName<" + CreateName({ Registration<T>::containingNamespace }, Registration<T>::name) + ">";
        static inline const String containingNamespace = "Atmos::Asset";
        static inline const String documentation = "This is a command.";
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        using CommandReturn = typename Registration<T>::Type;
        static CommandReturn ToCommandReturn(Arca::command_result_t<Type> fromArca, Arca::Reliquary&);
    };

    template<class T>
    void Registration<Asset::FindByName<T>>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(containingNamespace, name)
            .Constructor(&Management::template GenerateValue<&PullFromParameter<0, String>>, { "const string &in" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .template Property<&Type::name>("string", "name")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&Chroma::Identity<Type>, &ToCommandReturn>(engine, documentationManager);
    }

    template<class T>
    auto Registration<Asset::FindByName<T>>::ToCommandReturn(Arca::command_result_t<Type> fromArca, Arca::Reliquary&) -> CommandReturn
    {
        return { fromArca };
    }
}