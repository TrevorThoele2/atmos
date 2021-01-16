#pragma once

#include "RestartSound.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Audio::RestartSound>
    {
        using Type = Audio::RestartSound;
        using Management = ObjectManagement<Type>;

        static String Name() { return "RestartSound"; }
        static String ContainingNamespace() { return "Atmos::Audio"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}