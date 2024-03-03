#pragma once

#include "SoundFinished.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Audio::SoundFinished>
    {
        using Type = Audio::SoundFinished;
        using Management = ObjectManagement<Type>;

        static String Name() { return "SoundFinished"; }
        static String ContainingNamespace() { return "Atmos::Audio"; }
        static String Documentation() { return "This is a signal."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}