#pragma once

#include "ChangeTextCore.h"

#include "AngelScriptObjectRegistration.h"
#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"
#include "AngelScriptCommand.h"

#include "String.h"

#include <Chroma/Identity.h>

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    struct ChangeTextString
    {
        Arca::RelicID id = 0;

        String to;
    };

    struct ChangeFontAsset
    {
        Arca::RelicID id = 0;

        Arca::Index<Asset::Font> to;
    };

    struct ChangeWrapWidth
    {
        Arca::RelicID id = 0;

        float to;
    };

    struct ChangeBold
    {
        Arca::RelicID id = 0;

        bool to;
    };

    struct ChangeItalics
    {
        Arca::RelicID id = 0;

        bool to;
    };
    
    template<>
    struct Registration<ChangeTextString>
    {
        using Type = ChangeTextString;

        static String Name() { return "ChangeTextString"; }
        static String ContainingNamespace() { return "Atmos::Render"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        static Render::ChangeTextCore ToArca(Type fromAngelScript);
    };

    template<>
    struct Registration<ChangeFontAsset>
    {
        using Type = ChangeFontAsset;

        static String Name() { return "ChangeFontAsset"; }
        static String ContainingNamespace() { return "Atmos::Render"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        static Render::ChangeTextCore ToArca(Type fromAngelScript);
    };

    template<>
    struct Registration<ChangeWrapWidth>
    {
        using Type = ChangeWrapWidth;

        static String Name() { return "ChangeWrapWidth"; }
        static String ContainingNamespace() { return "Atmos::Render"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        static Render::ChangeTextCore ToArca(Type fromAngelScript);
    };

    template<>
    struct Registration<ChangeBold>
    {
        using Type = ChangeBold;

        static String Name() { return "ChangeBold"; }
        static String ContainingNamespace() { return "Atmos::Render"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        static Render::ChangeTextCore ToArca(Type fromAngelScript);
    };

    template<>
    struct Registration<ChangeItalics>
    {
        using Type = ChangeItalics;

        static String Name() { return "ChangeItalics"; }
        static String ContainingNamespace() { return "Atmos::Render"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        static Render::ChangeTextCore ToArca(Type fromAngelScript);
    };
}