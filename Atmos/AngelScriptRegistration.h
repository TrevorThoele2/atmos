#pragma once

#include "AngelScriptObjectType.h"

#include "ScriptDocumentationManager.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    void RegisterAll(asIScriptEngine& engine);
    void RegisterAll(asIScriptEngine& engine, DocumentationManager& documentationManager);

    template<class T>
    struct Registration;
}