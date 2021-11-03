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

        static String Name() { return "Create<class T>"; }
        static String ContainingNamespace() { return "Arca"; }
        static String Documentation() { return "This is a signal. Needs to be used with explicit specializations."; }
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<>
    struct Registration<GenericArcaCreateChild>
    {
        using Type = GenericArcaCreateChild;

        static String Name() { return "CreateChild<class T>"; }
        static String ContainingNamespace() { return "Arca"; }
        static String Documentation() { return "This is a command. Needs to be used with explicit specializations."; }
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<>
    struct Registration<GenericArcaIdentifiedCreate>
    {
        using Type = GenericArcaIdentifiedCreate;

        static String Name() { return "IdentifiedCreate<class T>"; }
        static String ContainingNamespace() { return "Arca"; }
        static String Documentation() { return "This is a command. Needs to be used with explicit specializations."; }
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<>
    struct Registration<GenericArcaIdentifiedCreateChild>
    {
        using Type = GenericArcaIdentifiedCreate;

        static String Name() { return "IdentifiedCreateChild<class T>"; }
        static String ContainingNamespace() { return "Arca"; }
        static String Documentation() { return "This is a command. Needs to be used with explicit specializations."; }
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<class T>
    struct Registration<Arca::Create<T>>
    {
        using Type = Arca::Create<T>;

        static String Name() { return "Create<" + CreateName({ Registration<T>::ContainingNamespace() }, Registration<T>::Name()) + ">"; }
        static String ContainingNamespace() { return "Arca"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;
    };

    template<>
    struct Registration<Arca::Created>
    {
        using Type = Arca::Created;

        static String Name() { return "Created"; }
        static String ContainingNamespace() { return "Arca"; }
        static String Documentation() { return "This is a signal."; }
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

        static String Name() { return "CreatedKnown<class T>"; }
        static String ContainingNamespace() { return "Arca"; }
        static String Documentation() { return "This is a signal. Needs to be used with explicit specializations."; }
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<class T>
    struct Registration<Arca::CreatedKnown<T>>
    {
        using Type = Arca::CreatedKnown<T>;

        static String Name() { return "CreatedKnown<" + CreateName({ Registration<T>::ContainingNamespace() }, Registration<T>::Name()) + ">"; }
        static String ContainingNamespace() { return "Arca"; }
        static String Documentation() { return "This is a signal."; }
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

        template<class T, class CommandT>
        struct ArcaCreateCommand
        {
            ValueTypeRegistration<CommandT> registration;

            ArcaCreateCommand();

            template<class ParameterTypes>
            static constexpr auto GenerateValue()
            {
                using ParameterProviders = Chroma::to_non_type_with_index<ParameterTypes, Detail::ToParameterProvidersIterator>;
                return ParameterProviders::template ForwardArguments<typename Detail::GenerateValueForwarder<CommandT>::Do>::nonType;
            }

            void Actualize(asIScriptEngine& engine, DocumentationManager& documentationManager);
        };

        template<class T, class CommandT>
        ArcaCreateCommand<T, CommandT>::ArcaCreateCommand() :
            registration(Registration<CommandT>::ContainingNamespace(), Registration<CommandT>::Name())
        {}

        template<class T, class CommandT>
        void ArcaCreateCommand<T, CommandT>::Actualize(asIScriptEngine& engine, DocumentationManager& documentationManager)
        {
            using Management = ObjectManagement<CommandT>;

            registration
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

            ValueTypeRegistration<SignalT>(Registration<SignalT>::ContainingNamespace(), Registration<SignalT>::Name())
                .Constructor(&Management::template GenerateValue<&PullFromParameter<0, Arca::Index<T>>>, constructorParameters)
                .Actualize(engine, documentationManager);

            RegisterSignalHandler<&Chroma::Identity<SignalT>>(engine, documentationManager);
        }
    }

    template<class T>
    class ArcaCreateRelicRegistration
    {
    public:
        template<class... ParameterTypes>
        ArcaCreateRelicRegistration<T>& Constructor(const std::vector<String>& constructorParameters);
        void Actualize(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        template<class CommandT>
        using Command = Detail::ArcaCreateCommand<T, CommandT>;

        using Create = Command<Arca::Create<T>>;
        Create create;
    };

    template<class T>
    template<class... ParameterTypes>
    ArcaCreateRelicRegistration<T>& ArcaCreateRelicRegistration<T>::Constructor(const std::vector<String>& constructorParameters)
    {
        using VariadicParameterTypes = Chroma::VariadicTemplate<ParameterTypes...>;
        
        static constexpr auto generateValue = Create::template GenerateValue<VariadicParameterTypes>();
        create.registration.Constructor(generateValue, constructorParameters);

        return *this;
    }

    template<class T>
    void ArcaCreateRelicRegistration<T>::Actualize(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        create.Actualize(engine, documentationManager);
    }

    template<class T>
    class ArcaCreateShardRegistration
    {
    public:
        template<class... ParameterTypes>
        ArcaCreateShardRegistration<T>& Constructor(const std::vector<String>& constructorParameters);
        void Actualize(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        template<class CommandT>
        using Command = Detail::ArcaCreateCommand<T, CommandT>;

        using Create = Command<Arca::Create<T>>;
        Create create;
    };

    template<class T>
    template<class... ParameterTypes>
    ArcaCreateShardRegistration<T>& ArcaCreateShardRegistration<T>::Constructor(const std::vector<String>& constructorParameters)
    {
        using VariadicParameterTypes = Chroma::VariadicTemplate<ParameterTypes...>;

        {
            using UseParameterTypes = typename VariadicParameterTypes::template Prepend<Arca::RelicID>::Type;

            static constexpr auto generateValue = Create::template GenerateValue<UseParameterTypes>();

            auto useConstructorParameters = std::vector<String>{ "Arca::RelicID id" };
            useConstructorParameters.insert(
                useConstructorParameters.end(), constructorParameters.begin(), constructorParameters.end());
            create.registration.Constructor(generateValue, useConstructorParameters);
        }

        return *this;
    }

    template<class T>
    void ArcaCreateShardRegistration<T>::Actualize(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        create.Actualize(engine, documentationManager);
    }

    template<class T>
    void RegisterArcaCreated(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        const auto useConstructorParameters =
            std::vector<String>{ CreateName({ Registration<T>::ContainingNamespace() }, Registration<T>::Name()) + " index" };
        Detail::DoRegisterArcaCreated<T, Arca::CreatedKnown<T>>(
            useConstructorParameters, engine, documentationManager);
    }
}