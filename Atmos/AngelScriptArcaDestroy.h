#pragma once

#include <Arca/Destroy.h>
#include <Arca/Destroying.h>

#include "AngelScriptObjectRegistration.h"
#include "AngelScriptFactory.h"
#include "AngelScriptCommand.h"
#include "AngelScriptSignal.h"

#include <Chroma/Identity.h>

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    class GenericArcaDestroy
    {};

    template<>
    struct Registration<GenericArcaDestroy>
    {
        using Type = GenericArcaDestroy;

        static String Name() { return "Destroy<class T>"; }
        static String ContainingNamespace() { return "Arca"; }
        static String Documentation() { return "This is a command. Needs to be used with explicit specializations."; }
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<class T>
    struct Registration<Arca::Destroy<T>>
    {
        using Type = Arca::Destroy<T>;

        static String Name() { return "Destroy<" + CreateName({ Registration<T>::ContainingNamespace() }, Registration<T>::Name()) + ">"; }
        static String ContainingNamespace() { return "Arca"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;
    };

    template<>
    struct Registration<Arca::Destroying>
    {
        using Type = Arca::Destroying;

        static String Name() { return "Destroying"; }
        static String ContainingNamespace() { return "Arca"; }
        static String Documentation() { return "This is a signal."; }
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    class GenericArcaDestroyingKnown
    {};

    template<>
    struct Registration<GenericArcaDestroyingKnown>
    {
        using Type = GenericArcaDestroyingKnown;

        static String Name() { return "DestroyingKnown<class T>"; }
        static String ContainingNamespace() { return "Arca"; }
        static String Documentation() { return "This is a signal. Needs to be used with explicit specializations."; }
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<class T>
    struct Registration<Arca::DestroyingKnown<T>>
    {
        using Type = Arca::CreatedKnown<T>;

        static String Name() { return "DestroyingKnown<" + CreateName({ Registration<T>::ContainingNamespace() }, Registration<T>::Name()) + ">"; }
        static String ContainingNamespace() { return "Arca"; }
        static String Documentation() { return "This is a signal."; }
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;
    };

    namespace Detail
    {
        template<class T>
        void DoRegisterArcaDestroy(asIScriptEngine& engine, DocumentationManager& documentationManager)
        {
            using CommandT = Arca::Destroy<T>;

            using Management = ObjectManagement<CommandT>;

            ValueTypeRegistration<CommandT>(Registration<CommandT>::ContainingNamespace(), Registration<CommandT>::Name())
                .Constructor(
                    &Management::template GenerateValue<&PullFromParameter<0, Arca::RelicID>>,
                    { "Arca::RelicID id" })
                .Constructor(
                    &Management::template GenerateValue<&PullFromParameter<0, Arca::Index<T>>>,
                    { CreateName({Registration<T>::ContainingNamespace()}, Registration<T>::Name()) + " index" })
                .CopyConstructor(&Management::GenerateValueFromCopy)
                .Destructor(&Management::DestructValue)
                .CopyAssignment(&Management::CopyAssign)
                .Actualize(engine, documentationManager);

            RegisterCommandHandler<&Chroma::Identity<CommandT>>(engine, documentationManager);
        }

        template<class T>
        void DoRegisterArcaDestroying(asIScriptEngine& engine, DocumentationManager& documentationManager)
        {
            using SignalT = Arca::DestroyingKnown<T>;

            using Management = ObjectManagement<SignalT>;

            ValueTypeRegistration<SignalT>(Registration<SignalT>::ContainingNamespace(), Registration<SignalT>::Name())
                .Constructor(
                    &Management::template GenerateValue<&PullFromParameter<0, Arca::Index<T>>>,
                    { CreateName({Registration<T>::ContainingNamespace()}, Registration<T>::Name()) + " index" })
                .CopyConstructor(&Management::GenerateValueFromCopy)
                .Destructor(&Management::DestructValue)
                .CopyAssignment(&Management::CopyAssign)
                .Actualize(engine, documentationManager);

            RegisterSignalHandler<&Chroma::Identity<SignalT>>(engine, documentationManager);
        }
    }

    template<class T>
    void RegisterArcaDestroyRelic(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        Detail::DoRegisterArcaDestroy<T>(engine, documentationManager);
    }

    template<class T>
    void RegisterArcaDestroyShard(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        Detail::DoRegisterArcaDestroy<T>(engine, documentationManager);
    }

    template<class T>
    void RegisterArcaDestroying(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        Detail::DoRegisterArcaDestroying<T>(engine, documentationManager);
    }
}