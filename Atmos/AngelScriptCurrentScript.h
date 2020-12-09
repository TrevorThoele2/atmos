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
        static inline const String containingNamespace = Namespaces::Atmos::name;
        static const ObjectType objectType = ObjectType::Value;

        using Management = GlobalManagement;

        static void RegisterTo(asIScriptEngine& engine);
    private:
        [[nodiscard]] static Arca::Index<Script> Retrieve(Arca::Reliquary* reliquary);
    };
}