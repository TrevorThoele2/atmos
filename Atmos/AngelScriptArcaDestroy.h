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

        static inline const String name = "Destroy<class T>";
        static inline const String containingNamespace = "Arca";
        static inline const String documentation = "This is a command. Needs to be used with explicit specializations.";
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<class T>
    struct Registration<Arca::Destroy<T>>
    {
        using Type = Arca::Destroy<T>;

        static inline const String name = "Destroy<" + CreateName({ Registration<T>::containingNamespace }, Registration<T>::name) + ">";
        static inline const String containingNamespace = "Arca";
        static inline const String documentation = "This is a command.";
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;
    };

    template<>
    struct Registration<Arca::Destroying>
    {
        using Type = Arca::Destroying;

        static inline const String name = "Destroying";
        static inline const String containingNamespace = "Arca";
        static inline const String documentation = "This is a signal.";
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

        static inline const String name = "DestroyingKnown<class T>";
        static inline const String containingNamespace = "Arca";
        static inline const String documentation = "This is a signal. Needs to be used with explicit specializations.";
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<class T>
    struct Registration<Arca::DestroyingKnown<T>>
    {
        using Type = Arca::CreatedKnown<T>;

        static inline const String name = "DestroyingKnown<" + CreateName({ Registration<T>::containingNamespace }, Registration<T>::name) + ">";
        static inline const String containingNamespace = "Arca";
        static inline const String documentation = "This is a signal.";
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

            ValueTypeRegistration<CommandT>(Registration<CommandT>::containingNamespace, Registration<CommandT>::name)
                .Constructor(
                    &Management::template GenerateValue<&PullFromParameter<0, Arca::RelicID>>,
                    { "Arca::RelicID id" })
                .Constructor(
                    &Management::template GenerateValue<&PullFromParameter<0, Arca::Index<T>>>,
                    { CreateName({Registration<T>::containingNamespace}, Registration<T>::name) + " index" })
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

            ValueTypeRegistration<SignalT>(Registration<SignalT>::containingNamespace, Registration<SignalT>::name)
                .Constructor(
                    &Management::template GenerateValue<&PullFromParameter<0, Arca::Index<T>>>,
                    { CreateName({Registration<T>::containingNamespace}, Registration<T>::name) + " index" })
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

        Detail::DoRegisterArcaDestroying<T>(engine, documentationManager);
    }
}