#pragma once

#include <Arca/RelicID.h>

#include "AngelScriptGlobalManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    class RelicID
    {};

    template<>
    struct Registration<RelicID>
    {
        using Type = RelicID;
        using Management = GlobalManagement;

        static inline const String name = "RelicID";
        static inline const String containingNamespace = Namespaces::Arca::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    };
}