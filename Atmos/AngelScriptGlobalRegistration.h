#pragma once

#include "AngelScriptResultVerification.h"
#include <angelscript.h>
#include "ScriptDocumentationManager.h"
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
        template<auto property>
        GlobalRegistration& ConstProperty(String type, String name);

        void Actualize(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        std::optional<String> containingNamespace;

        using RegistrationItem = std::function<void(asIScriptEngine&)>;
        std::vector<RegistrationItem> items;

        void AddItem(RegistrationItem&& item);
    };

    template<auto property>
    GlobalRegistration& GlobalRegistration::ConstProperty(String type, String name)
    {
        AddItem([type, name](asIScriptEngine& engine)
            {
                const auto declaration = "const " + type + " " + name;
                const auto result = engine.RegisterGlobalProperty(declaration.c_str(), property);
                VerifyResult(
                    result,
                    {
                        { "Declaration", declaration }
                    });
            });

        return *this;
    }
}