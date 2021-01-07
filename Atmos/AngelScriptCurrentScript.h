#pragma once

#include "Script.h"

#include "AngelScriptRegistration.h"
#include "AngelScriptGlobalManagement.h"

#include "String.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    class CurrentScript
    {};

    template<>
    struct Registration<CurrentScript>
    {
        static String Name() { return "CurrentScript"; }
        static String ContainingNamespace() { return "Atmos"; }
        static String Documentation() { return "Represents the currently executing script."; }
        static const ObjectType objectType = ObjectType::Value;

        using Management = GlobalManagement;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static Arca::Index<Script> Retrieve(Arca::Reliquary* reliquary);
    };
}