#pragma once

#include "AngelScriptResultVerification.h"
#include <angelscript.h>
#include "String.h"

namespace Atmos::Scripting::Angel
{
    class GlobalRegistration
    {
    public:
        using GenericFunction = void(*)(asIScriptGeneric*);
    public:
        GlobalRegistration();
        GlobalRegistration(const String& containingNamespace);

        GlobalRegistration& Function(GenericFunction function, const String& returnType, const String& name, const std::vector<String>& parameters);
        GlobalRegistration& Typedef(const String& alias, const String& original);
        GlobalRegistration& Funcdef(const String& returnType, const String& name, const std::vector<String>& parameters);

        void Actualize(asIScriptEngine& engine);
    private:
        std::optional<String> containingNamespace;

        using RegistrationItem = std::function<void(asIScriptEngine&)>;
        std::vector<RegistrationItem> items;

        void AddItem(RegistrationItem&& item);
    };
}