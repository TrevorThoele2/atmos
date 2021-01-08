#pragma once

#include "FindImagesByBox.h"
#include "FindLinesByBox.h"
#include "FindGridRegionsByBox.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

#include <Arca/CommandResult.h>

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Render::FindImagesByBox>
    {
        using Type = Render::FindImagesByBox;
        using Management = ObjectManagement<Type>;

        static String Name() { return "FindImagesByBox"; }
        static String ContainingNamespace() { return "Atmos::Render"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        static std::vector<Arca::RelicID> ToCommandReturn(Arca::command_result_t<Type> fromArca, Arca::Reliquary&);
    };

    template<>
    struct Registration<Render::FindLinesByBox>
    {
        using Type = Render::FindLinesByBox;
        using Management = ObjectManagement<Type>;

        static String Name() { return "FindLinesByBox"; }
        static String ContainingNamespace() { return "Atmos::Render"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        static std::vector<Arca::RelicID> ToCommandReturn(Arca::command_result_t<Type> fromArca, Arca::Reliquary&);
    };

    template<>
    struct Registration<Render::FindGridRegionsByBox>
    {
        using Type = Render::FindGridRegionsByBox;
        using Management = ObjectManagement<Type>;

        static String Name() { return "FindGridRegionsByBox"; }
        static String ContainingNamespace() { return "Atmos::Render"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        static std::vector<Arca::RelicID> ToCommandReturn(Arca::command_result_t<Type> fromArca, Arca::Reliquary&);
    };
}