#pragma once

#include "AngelScriptResultVerification.h"
#include <angelscript.h>
#include "ScriptDocumentationManager.h"
#include "String.h"

namespace Atmos::Scripting::Angel
{
    template<class T>
    class EnumRegistration
    {
    public:
        EnumRegistration(const String& name);
        EnumRegistration(const String& containingNamespace, const String& name);

        EnumRegistration& Value(const String& name, T value);

        void Actualize(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        std::optional<String> containingNamespace;
        String representationName;

        using RegistrationItem = std::function<void(asIScriptEngine&, const String&)>;
        std::vector<RegistrationItem> items;

        void AddItem(RegistrationItem&& item);
    };

    template<class T>
    EnumRegistration<T>::EnumRegistration(const String& name) : representationName(name)
    {}

    template<class T>
    EnumRegistration<T>::EnumRegistration(const String& containingNamespace, const String& name) :
        containingNamespace(containingNamespace), representationName(name)
    {}

    template<class T>
    EnumRegistration<T>& EnumRegistration<T>::Value(const String& name, T value)
    {
        AddItem([name, value](asIScriptEngine& engine, const String& representationName)
            {
                VerifyResult(engine.RegisterEnumValue(
                    representationName.c_str(),
                    name.c_str(),
                    static_cast<int>(value)));
            });

        return *this;
    }

    template<class T>
    void EnumRegistration<T>::Actualize(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        const auto useNamespace = containingNamespace
            ? *containingNamespace
            : "";

        VerifyResult(engine.SetDefaultNamespace(useNamespace.c_str()));

        VerifyResult(engine.RegisterEnum(representationName.c_str()));

        for (auto& item : items)
            item(engine, representationName);
    }

    template<class T>
    void EnumRegistration<T>::AddItem(RegistrationItem&& item)
    {
        items.push_back(std::move(item));
    }
}