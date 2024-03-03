#pragma once

#include "GenerateRandomNumber.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

#include "AngelScriptScript.h"
#include "AngelScriptCommand.h"
#include "AngelScriptObjectRegistration.h"

#include <angelscript.h>

#include <Chroma/Identity.h>

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<class T>
    struct Registration<Random::GenerateNumber<T>>
    {
        using Type = Random::GenerateNumber<T>;
        using Management = ObjectManagement<Type>;

        static String Name() { return "Generate" + TypeString(); }
        static String ContainingNamespace() { return "Atmos::Random"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        static T ToCommandReturn(Arca::command_result_t<Type> fromArca, Arca::Reliquary&);

        [[nodiscard]] static String TypeString();
    };

    template<class T>
    void Registration<Random::GenerateNumber<T>>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        const auto rangeTypeString = CreateName({ Registration<Range<T>>::ContainingNamespace() }, Registration<Range<T>>::Name());

        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .Constructor(
                &Management::template GenerateValue<&PullFromParameter<0, Range<T>>>,
                { rangeTypeString + " range" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .template Property<&Type::range>(rangeTypeString, "range")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&Chroma::Identity<Type>, &ToCommandReturn>(engine, documentationManager);
    }

    template<class T>
    T Registration<Random::GenerateNumber<T>>::ToCommandReturn(Arca::command_result_t<Type> fromArca, Arca::Reliquary&)
    {
        return fromArca;
    }

    template<class T>
    String Registration<Random::GenerateNumber<T>>::TypeString()
    {
        const auto baseName = Registration<T>::Name();
        return Chroma::ToUppercase(baseName.substr(0, 1)) + baseName.substr(1, String::npos);
    }

    template<>
    struct Registration<Random::GenerateBool>
    {
        using Type = Random::GenerateBool;
        using Management = ObjectManagement<Type>;

        static String Name() { return "GenerateBool"; }
        static String ContainingNamespace() { return "Atmos::Random"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        static bool ToCommandReturn(Arca::command_result_t<Type> fromArca, Arca::Reliquary&);
    };
}