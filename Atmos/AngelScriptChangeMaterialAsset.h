#pragma once

#include "ChangeMaterialAsset.h"

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
    template<>
    struct Registration<Render::ChangeMaterialAsset>
    {
        using Type = Render::ChangeMaterialAsset;
        
        static String Name() { return "ChangeMaterialAsset"; }
        static String ContainingNamespace() { return "Atmos::Render"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}