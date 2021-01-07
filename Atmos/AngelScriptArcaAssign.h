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

        static String Name() { return "AssignCopy<class T>"; }
        static String ContainingNamespace() { return "Arca"; }
        static String Documentation() { return "This is a command. Needs to be used with explicit specializations."; }
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<>
    struct Registration<GenericArcaAssignMove>
    {
        using Type = GenericArcaAssignMove;

        static String Name() { return "AssignMove<class T>"; }
        static String ContainingNamespace() { return "Arca"; }
        static String Documentation() { return "This is a command. Needs to be used with explicit specializations."; }
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<class T>
    struct Registration<Arca::AssignCopy<T>>
    {
        using Type = Arca::AssignCopy<T>;

        static String Name() { return "AssignCopy<" + CreateName({ Registration<T>::ContainingNamespace() }, Registration<T>::Name()) + ">"; }
        static String ContainingNamespace() { return "Arca"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;
    };

    template<class T>
    struct Registration<Arca::AssignMove<T>>
    {
        using Type = Arca::AssignMove<T>;

        static String Name() { return "AssignMove<" + CreateName({ Registration<T>::ContainingNamespace() }, Registration<T>::Name()) + ">"; }
        static String ContainingNamespace() { return "Arca"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;
    };

    namespace Detail
    {
        template<class T, class CommandT, class... Args>
        void DoRegisterArcaAssign(
            const std::vector<String>& constructorParameters, asIScriptEngine& engine, DocumentationManager& documentationManager)
        {
            using Management = ObjectManagement<CommandT>;

            ValueTypeRegistration<CommandT>(Registration<CommandT>::ContainingNamespace(), Registration<CommandT>::Name())
                .Constructor(&Management::template GenerateValueBasic<Args...>, constructorParameters)
                .CopyConstructor(&Management::GenerateValueFromCopy)
                .Destructor(&Management::DestructValue)
                .CopyAssignment(&Management::CopyAssign)
                .Actualize(engine, documentationManager);

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
    void RegisterArcaAssignRelic(
        const std::vector<String>& constructorParameters, asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        auto useConstructorParameters = std::vector<String>{ "Arca::RelicID id" };
        useConstructorParameters.insert(useConstructorParameters.end(), constructorParameters.begin(), constructorParameters.end());

        Detail::DoRegisterArcaAssign<T, Arca::AssignCopy<T>, Arca::RelicID, Args...>(
            useConstructorParameters, engine, documentationManager);
        Detail::DoRegisterArcaAssign<T, Arca::AssignMove<T>, Arca::RelicID, Args...>(
            useConstructorParameters, engine, documentationManager);
    }
}