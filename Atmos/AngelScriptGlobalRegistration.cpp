#include "AngelScriptGlobalRegistration.h"

namespace Atmos::Scripting::Angel
{
    GlobalRegistration::GlobalRegistration() = default;

    GlobalRegistration::GlobalRegistration(String containingNamespace) : containingNamespace(containingNamespace)
    {}

    GlobalRegistration& GlobalRegistration::Function(
        GenericFunction function, String returnType, String name, std::vector<String> parameters)
    {
        AddItem([function, returnType, name, parameters](asIScriptEngine& engine)
            {
                const auto declaration = returnType + " " + name + "(" + Chroma::Join(", ", parameters.begin(), parameters.end()) + ")";

                VerifyResult(engine.RegisterGlobalFunction(
                    declaration.c_str(),
                    asFUNCTION(function),
                    asCALL_GENERIC));
            });

        return *this;
    }

    GlobalRegistration& GlobalRegistration::Typedef(String alias, String original)
    {
        AddItem([alias, original](asIScriptEngine& engine)
            {
                VerifyResult(engine.RegisterTypedef(alias.c_str(), original.c_str()));
            });

        return *this;
    }

    GlobalRegistration& GlobalRegistration::Funcdef(String returnType, String name, std::vector<String> parameters)
    {
        AddItem([returnType, name, parameters](asIScriptEngine& engine)
            {
                const auto declaration = returnType + " " + name + "(" + Chroma::Join(", ", parameters.begin(), parameters.end()) + ")";
                VerifyResult(engine.RegisterFuncdef(declaration.c_str()));
            });

        return *this;
    }

    void GlobalRegistration::Actualize(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        const auto useNamespace = containingNamespace
            ? *containingNamespace
            : "";

        VerifyResult(engine.SetDefaultNamespace(useNamespace.c_str()));

        for (auto& item : items)
            item(engine);
    }

    void GlobalRegistration::AddItem(RegistrationItem&& item)
    {
        items.push_back(std::move(item));
    }
}