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

                const auto result = engine.RegisterGlobalFunction(
                    declaration.c_str(),
                    asFUNCTION(function),
                    asCALL_GENERIC);
                VerifyResult(
                    result,
                    {
                        { "Declaration", declaration }
                    });
            });

        return *this;
    }

    GlobalRegistration& GlobalRegistration::Typedef(String alias, String original)
    {
        AddItem([alias, original](asIScriptEngine& engine)
            {
                const auto result = engine.RegisterTypedef(alias.c_str(), original.c_str());
                VerifyResult(
                    result,
                    {
                        { "Alias", alias },
                        { "Original", original }
                    });
            });

        return *this;
    }

    GlobalRegistration& GlobalRegistration::Funcdef(String returnType, String name, std::vector<String> parameters)
    {
        AddItem([returnType, name, parameters](asIScriptEngine& engine)
            {
                const auto declaration = returnType + " " + name + "(" + Chroma::Join(", ", parameters.begin(), parameters.end()) + ")";
                const auto result = engine.RegisterFuncdef(declaration.c_str());
                VerifyResult(
                    result,
                    {
                        { "Declaration", declaration }
                    });
            });

        return *this;
    }

    void GlobalRegistration::Actualize(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        const auto useNamespace = containingNamespace
            ? *containingNamespace
            : "";

        const auto result = engine.SetDefaultNamespace(useNamespace.c_str());
        VerifyResult(
            result,
            {
                { "Namespace", useNamespace }
            });

        for (auto& item : items)
            item(engine);
    }

    void GlobalRegistration::AddItem(RegistrationItem&& item)
    {
        items.push_back(std::move(item));
    }
}