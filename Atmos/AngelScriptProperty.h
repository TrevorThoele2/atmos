#pragma once

#include "Property.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Property>
    {
        using Type = Property;
        using Management = ObjectManagement<Type>;

        static String Name() { return "Property"; }
        static String ContainingNamespace() { return "Atmos"; }
        static String Documentation() { return "Generic key value pair."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}