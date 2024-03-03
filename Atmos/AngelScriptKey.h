#pragma once

#include "Key.h"

#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Input::Key>
    {
        static String Name() { return "Key"; }
        static String ContainingNamespace() { return "Atmos::Input"; }
        static String Documentation() { return "Represents a key. They can be either down or up at any given moment in time."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}