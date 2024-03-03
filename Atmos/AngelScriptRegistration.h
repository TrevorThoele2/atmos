#pragma once

#include <optional>

#include <angelscript.h>
#include "AngelScriptObjectType.h"

#include "ScriptDocumentationManager.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    void RegisterAll(asIScriptEngine& engine);
    void RegisterAll(asIScriptEngine& engine, DocumentationManager& documentationManager);

    template<class T>
    struct Registration;

    struct RegistrationTemplate
    {
        String name;
        String containingNamespace;
        String documentation;
        ObjectType objectType;
        std::optional<int> typeID;
    };

    template<class T>
    [[nodiscard]] RegistrationTemplate RegistrationTemplateFor()
    {
        using Registration = Registration<T>;
        return
        {
            Registration::Name(),
            Registration::ContainingNamespace(),
            Registration::Documentation(),
            Registration::objectType,
            Registration::typeID
        };
    }
}