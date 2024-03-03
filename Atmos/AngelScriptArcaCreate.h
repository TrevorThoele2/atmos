#pragma once

#include <Arca/Reliquary.h>
#include <Arca/Create.h>
#include <Arca/Created.h>

#include "AngelScriptObjectRegistration.h"
#include "AngelScriptFactory.h"
#include "AngelScriptCommand.h"
#include "AngelScriptSignal.h"

#include <Chroma/Identity.h>
#include <Chroma/Transform.h>

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    class GenericArcaCreate
    {};

    class GenericArcaCreateChild
    {};

    class GenericArcaIdentifiedCreate
    {};

    class GenericArcaIdentifiedCreateChild
    {};

    template<>
    struct Registration<GenericArcaCreate>
    {
        using Type = GenericArcaCreate;

        static inline const String name = "Create<class T>";
        static inline const String containingNamespace = "Arca";
        static inline const String documentation = "This is a signal. Needs to be used with explicit specializations.";
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<>
    struct Registration<GenericArcaCreateChild>
    {
        using Type = GenericArcaCreateChild;

        static inline const String name = "CreateChild<class T>";
        static inline const String containingNamespace = "Arca";
        static inline const String documentation = "This is a command. Needs to be used with explicit specializations.";
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<>
    struct Registration<GenericArcaIdentifiedCreate>
    {
        using Type = GenericArcaIdentifiedCreate;

        static inline const String name = "IdentifiedCreate<class T>";
        static inline const String containingNamespace = "Arca";
        static inline const String documentation = "This is a command. Needs to be used with explicit specializations.";
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<>
    struct Registration<GenericArcaIdentifiedCreateChild>
    {
        using Type = GenericArcaIdentifiedCreate;

        static inline const String name = "IdentifiedCreateChild<class T>";
        static inline const String containingNamespace = "Arca";
        static inline const String documentation = "This is a command. Needs to be used with explicit specializations.";
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<class T>
    struct Registration<Arca::Create<T>>
    {
        using Type = Arca::Create<T>;

        static inline const String name = "Create<" + CreateName({ Registration<T>::containingNamespace }, Registration<T>::name) + ">";
        static inline const String containingNamespace = "Arca";
        static inline const String documentation = "This is a command.";
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;
    };

    template<class T>
    struct Registration<Arca::CreateChild<T>>
    {
        using Type = Arca::Create<T>;

        static inline const String name = "CreateChild<" + CreateName({ Registration<T>::containingNamespace }, Registration<T>::name) + ">";
        static inline const String containingNamespace = "Arca";
        static inline const String documentation = "This is a command.";
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;
    };

    template<class T>
    struct Registration<Arca::IdentifiedCreate<T>>
    {
        using Type = Arca::IdentifiedCreate<T>;

        static inline const String name = "IdentifiedCreate<" + CreateName({ Registration<T>::containingNamespace }, Registration<T>::name) + ">";
        static inline const String containingNamespace = "Arca";
        static inline const String documentation = "This is a command.";
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;
    };

    template<class T>
    struct Registration<Arca::IdentifiedCreateChild<T>>
    {
        using Type = Arca::IdentifiedCreateChild<T>;

        static inline const String name = "IdentifiedCreateChild<" + CreateName({ Registration<T>::containingNamespace }, Registration<T>::name) + ">";
        static inline const String containingNamespace = "Arca";
        static inline const String documentation = "This is a command.";
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;
    };

    template<>
    struct Registration<Arca::Created>
    {
        using Type = Arca::Created;

        static inline const String name = "Created";
        static inline const String containingNamespace = "Arca";
        static inline const String documentation = "This is a signal.";
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    class GenericArcaCreatedKnown
    {};

    template<>
    struct Registration<GenericArcaCreatedKnown>
    {
        using Type = GenericArcaIdentifiedCreate;

        static inline const String name = "CreatedKnown<class T>";
        static inline const String containingNamespace = "Arca";
        static inline const String documentation = "This is a signal. Needs to be used with explicit specializations.";
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<class T>
    struct Registration<Arca::CreatedKnown<T>>
    {
        using Type = Arca::CreatedKnown<T>;

        static inline const String name = "CreatedKnown<" + CreateName({ Registration<T>::containingNamespace }, Registration<T>::name) + ">";
        static inline const String containingNamespace = "Arca";
        static inline const String documentation = "This is a signal.";
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;
    };

    namespace Detail
    {
        template<class CommandT>
        struct GenerateValueForwarder
        {
            template<auto... parameterProviders>
            struct Do
            {
                static constexpr auto nonType = &ObjectManagement<CommandT>::template GenerateValue<parameterProviders...>;
            };
        };

        template<class T, class CommandT, class ParameterTypes>
        void DoRegisterArcaCreate(
            const std::vector<String>& constructorParameters, asIScriptEngine& engine, DocumentationManager& documentationManager)
        {
            using Management = ObjectManagement<CommandT>;
            using ParameterProviders = Chroma::to_non_type_with_index<ParameterTypes, ToParameterProvidersIterator>;

            static constexpr auto generateValue = ParameterProviders::template ForwardArguments<typename GenerateValueForwarder<CommandT>::Do>::nonType;

            ValueTypeRegistration<CommandT>(Registration<CommandT>::containingNamespace, Registration<CommandT>::name)
                .Constructor(generateValue, constructorParameters)
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

            RegisterCommandHandler<&Chroma::Identity<CommandT>, toCommandReturn>(engine, documentationManager);
        }

        template<class T, class SignalT>
        void DoRegisterArcaCreated(
            const std::vector<String>& constructorParameters, asIScriptEngine& engine, DocumentationManager& documentationManager)
        {
            using Management = ObjectManagement<SignalT>;

            ValueTypeRegistration<SignalT>(Registration<SignalT>::containingNamespace, Registration<SignalT>::name)
                .Constructor(&Management::template GenerateValue<&PullFromParameter<0, Arca::Index<T>>>, constructorParameters)
                .CopyConstructor(&Management::GenerateValueFromCopy)
                .Destructor(&Management::DestructValue)
                .CopyAssignment(&Management::CopyAssign)
                .Actualize(engine, documentationManager);

            RegisterSignalHandler<&Chroma::Identity<SignalT>>(engine, documentationManager);
        }
    }

    template<class T, class ParameterTypes>
    void RegisterArcaCreateRelic(
        const std::vector<String>& constructorParameters, asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        {
            Detail::DoRegisterArcaCreate<T, Arca::Create<T>, ParameterTypes>(
                constructorParameters, engine, documentationManager);
        }

        {
            using UseParameterTypes = typename ParameterTypes::template Prepend<Arca::Handle>::Type;

            auto useConstructorParameters = std::vector<String>{ "Arca::Handle parent" };
            useConstructorParameters.insert(
                useConstructorParameters.end(), constructorParameters.begin(), constructorParameters.end());
            Detail::DoRegisterArcaCreate<T, Arca::CreateChild<T>, UseParameterTypes>(
                useConstructorParameters, engine, documentationManager);
        }

        {
            using UseParameterTypes = typename ParameterTypes::template Prepend<Arca::RelicID>::Type;

            auto useConstructorParameters = std::vector<String>{ "Arca::RelicID id" };
            useConstructorParameters.insert(
                useConstructorParameters.end(), constructorParameters.begin(), constructorParameters.end());
            Detail::DoRegisterArcaCreate<T, Arca::IdentifiedCreate<T>, UseParameterTypes>(
                useConstructorParameters, engine, documentationManager);
        }

        {
            using UseParameterTypes = typename ParameterTypes::template Prepend<Arca::RelicID, Arca::Handle>::Type;

            auto useConstructorParameters = std::vector<String>{ "Arca::RelicID id", "Arca::Handle parent" };
            useConstructorParameters.insert(
                useConstructorParameters.end(), constructorParameters.begin(), constructorParameters.end());
            Detail::DoRegisterArcaCreate<T, Arca::IdentifiedCreateChild<T>, UseParameterTypes>(
                useConstructorParameters, engine, documentationManager);
        }

        {
            const auto useConstructorParameters =
                std::vector<String>{ CreateName({ Registration<T>::containingNamespace }, Registration<T>::name) + " index" };
            Detail::DoRegisterArcaCreated<T, Arca::CreatedKnown<T>>(
                useConstructorParameters, engine, documentationManager);
        }
    }
}