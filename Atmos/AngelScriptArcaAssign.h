#pragma once

#include <Arca/Reliquary.h>
#include <Arca/Assign.h>

#include "AngelScriptObjectRegistration.h"
#include "AngelScriptFactory.h"
#include "AngelScriptCommand.h"

#include <Chroma/Identity.h>

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    class GenericArcaAssignCopy
    {};

    class GenericArcaAssignMove
    {};

    template<>
    struct Registration<GenericArcaAssignCopy>
    {
        using Type = GenericArcaAssignCopy;

        static inline const String name = "AssignCopy<class T>";
        static inline const String containingNamespace = Namespaces::Arca::name;
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine);
    };

    template<>
    struct Registration<GenericArcaAssignMove>
    {
        using Type = GenericArcaAssignMove;

        static inline const String name = "AssignMove<class T>";
        static inline const String containingNamespace = Namespaces::Arca::name;
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine);
    };

    template<class T>
    struct Registration<Arca::AssignCopy<T>>
    {
        using Type = Arca::AssignCopy<T>;

        static inline const String name = "AssignCopy<" + CreateName({ Registration<T>::containingNamespace }, Registration<T>::name) + ">";
        static inline const String containingNamespace = Namespaces::Arca::name;
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;
    };

    template<class T>
    struct Registration<Arca::AssignMove<T>>
    {
        using Type = Arca::AssignMove<T>;

        static inline const String name = "AssignMove<" + CreateName({ Registration<T>::containingNamespace }, Registration<T>::name) + ">";
        static inline const String containingNamespace = Namespaces::Arca::name;
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;
    };

    namespace Detail
    {
        template<class T, class CommandT, class... Args>
        void DoRegisterArcaAssign(const std::vector<String>& constructorParameters, asIScriptEngine& engine)
        {
            using Management = ObjectManagement<CommandT>;

            ValueTypeRegistration<CommandT>(Registration<CommandT>::containingNamespace, Registration<CommandT>::name)
                .Constructor(&Management::template GenerateValueBasic<Args...>, constructorParameters)
                .CopyConstructor(&Management::GenerateValueFromCopy)
                .Destructor(&Management::DestructValue)
                .CopyAssignment(&Management::CopyAssign)
                .Actualize(engine);

            using CommandReturn = typename Registration<T>::Type;

            constexpr static CommandReturn(*toCommandReturn)(Arca::command_result_t<CommandT>&&, Arca::Reliquary&) =
                [](Arca::command_result_t<CommandT>&& fromArca, Arca::Reliquary&) -> CommandReturn
            {
                return { fromArca };
            };

            RegisterCommandHandler<&Chroma::Identity<CommandT>, toCommandReturn>(engine);
        }
    }

    template<class T, class... Args>
    void RegisterArcaAssignRelic(const std::vector<String>& constructorParameters, asIScriptEngine& engine)
    {
        auto useConstructorParameters = std::vector<String>{ "Arca::RelicID id" };
        useConstructorParameters.insert(useConstructorParameters.end(), constructorParameters.begin(), constructorParameters.end());

        Detail::DoRegisterArcaAssign<T, Arca::AssignCopy<T>, Arca::RelicID, Args...>(useConstructorParameters, engine);
        Detail::DoRegisterArcaAssign<T, Arca::AssignMove<T>, Arca::RelicID, Args...>(useConstructorParameters, engine);
    }
}