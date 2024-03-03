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
        GlobalRegistration(String containingNamespace);

        GlobalRegistration& Function(GenericFunction function, String returnType, String name, std::vector<String> parameters);
        GlobalRegistration& Typedef(String alias, String original);
        GlobalRegistration& Funcdef(String returnType, String name, std::vector<String> parameters);

        void Actualize(asIScriptEngine& engine);
    private:
        std::optional<String> containingNamespace;

        using RegistrationItem = std::function<void(asIScriptEngine&)>;
        std::vector<RegistrationItem> items;

        void AddItem(RegistrationItem&& item);
    };
}