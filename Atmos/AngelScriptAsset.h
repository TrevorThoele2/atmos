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
        static inline const String containingNamespace = Namespaces::Atmos::Asset::name;
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine);
    };

    template<class T>
    struct Registration<Asset::FindByName<T>>
    {
        using Type = Asset::FindByName<T>;

        static inline const String name = "FindByName<" + CreateName({ Registration<T>::containingNamespace }, Registration<T>::name) + ">";
        static inline const String containingNamespace = Namespaces::Atmos::Asset::name;
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine);
    private:
        using CommandReturn = typename Registration<T>::Type;
        static CommandReturn ToCommandReturn(Arca::command_result_t<Type> fromArca, Arca::Reliquary&);
    };

    template<class T>
    void Registration<Asset::FindByName<T>>::RegisterTo(asIScriptEngine& engine)
    {
        ValueTypeRegistration<Type>(containingNamespace, name)
            .Constructor(&Management::template GenerateValue<&PullFromParameter<0, String>>, { "const string &in" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .template Property<&Type::name>("string", "name")
            .Actualize(engine);

        RegisterCommandHandler<&Chroma::Identity<Type>, &ToCommandReturn>(engine);
    }

    template<class T>
    auto Registration<Asset::FindByName<T>>::ToCommandReturn(Arca::command_result_t<Type> fromArca, Arca::Reliquary&) -> CommandReturn
    {
        return { fromArca };
    }
}