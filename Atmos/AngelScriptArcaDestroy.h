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
        static inline const String containingNamespace = Namespaces::Arca::name;
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine);
    };

    template<class T>
    struct Registration<Arca::Destroy<T>>
    {
        using Type = Arca::Destroy<T>;

        static inline const String name = "Destroy<" + CreateName({ Registration<T>::containingNamespace }, Registration<T>::name) + ">";
        static inline const String containingNamespace = Namespaces::Arca::name;
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;
    };

    template<>
    struct Registration<Arca::Destroying>
    {
        using Type = Arca::Destroying;

        static inline const String name = "Destroying";
        static inline const String containingNamespace = Namespaces::Arca::name;
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine);
    };

    class GenericArcaDestroyingKnown
    {};

    template<>
    struct Registration<GenericArcaDestroyingKnown>
    {
        using Type = GenericArcaDestroyingKnown;

        static inline const String name = "DestroyingKnown<class T>";
        static inline const String containingNamespace = Namespaces::Arca::name;
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine);
    };

    template<class T>
    struct Registration<Arca::DestroyingKnown<T>>
    {
        using Type = Arca::CreatedKnown<T>;

        static inline const String name = "DestroyingKnown<" + CreateName({ Registration<T>::containingNamespace }, Registration<T>::name) + ">";
        static inline const String containingNamespace = Namespaces::Arca::name;
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;
    };

    namespace Detail
    {
        template<class T>
        void DoRegisterArcaDestroy(asIScriptEngine& engine)
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
                .Actualize(engine);

            RegisterCommandHandler<&Chroma::Identity<CommandT>>(engine);
        }

        template<class T>
        void DoRegisterArcaDestroying(asIScriptEngine& engine)
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
                .Actualize(engine);

            RegisterSignalHandler<&Chroma::Identity<SignalT>>(engine);
        }
    }

    template<class T>
    void RegisterArcaDestroyRelic(asIScriptEngine& engine)
    {
        Detail::DoRegisterArcaDestroy<T>(engine);

        Detail::DoRegisterArcaDestroying<T>(engine);
    }
}