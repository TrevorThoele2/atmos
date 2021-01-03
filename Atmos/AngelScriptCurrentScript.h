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
        static inline const String name = "CurrentScript";
        static inline const String containingNamespace = "Atmos";
        static inline const String documentation = "Represents the currently executing script.";
        static const ObjectType objectType = ObjectType::Value;

        using Management = GlobalManagement;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static Arca::Index<Script> Retrieve(Arca::Reliquary* reliquary);
    };
}