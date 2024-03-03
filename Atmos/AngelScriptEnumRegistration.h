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
                const auto registerEnumValueResult = engine.RegisterEnumValue(
                    representationName.c_str(),
                    name.c_str(),
                    static_cast<int>(value));
                VerifyResult(
                    registerEnumValueResult,
                    {
                        { "Type", representationName.c_str() },
                        { "Name", name.c_str() }
                    });
            });

        return *this;
    }

    template<class T>
    void EnumRegistration<T>::Actualize(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        const auto useNamespace = containingNamespace
            ? *containingNamespace
            : "";

        const auto setDefaultNamespaceResult = engine.SetDefaultNamespace(useNamespace.c_str());
        VerifyResult(
            setDefaultNamespaceResult,
            {
                { "Namespace", useNamespace.c_str() }
            });

        const auto registerEnumResult = engine.RegisterEnum(representationName.c_str());
        VerifyResult(
            registerEnumResult,
            {
                { "Type", representationName.c_str() }
            });

        for (auto& item : items)
            item(engine, representationName);
    }

    template<class T>
    void EnumRegistration<T>::AddItem(RegistrationItem&& item)
    {
        items.push_back(std::move(item));
    }
}