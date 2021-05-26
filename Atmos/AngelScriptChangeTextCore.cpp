#include "AngelScriptChangeTextCore.h"

namespace Atmos::Scripting::Angel
{
    void Registration<ChangeTextString>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .Constructor(
                &Management::GenerateValue<
                &PullFromParameter<0, Arca::RelicID>,
                &PullFromParameter<1, String>>,
                { "Arca::RelicID id", "string to" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::id>("Arca::RelicID", "id")
            .Property<&Type::to>("string", "to")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&ToArca>(engine, documentationManager);
    }

    Render::ChangeTextCore Registration<ChangeTextString>::ToArca(Type fromAngelScript)
    {
        return Render::ChangeTextCore{fromAngelScript.id, fromAngelScript.to, {}, {}, {}, {}};
    }

    void Registration<ChangeFontAsset>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .Constructor(
                &Management::GenerateValue<
                &PullFromParameter<0, Arca::RelicID>,
                &PullFromParameter<1, Arca::Index<Asset::Font>>>,
                { "Arca::RelicID id", "Atmos::Asset::Font to" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::id>("Arca::RelicID", "id")
            .Property<&Type::to>("Atmos::Asset::Font", "to")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&ToArca>(engine, documentationManager);
    }

    Render::ChangeTextCore Registration<ChangeFontAsset>::ToArca(Type fromAngelScript)
    {
        return Render::ChangeTextCore{ fromAngelScript.id, {}, fromAngelScript.to, {}, {}, {} };
    }

    void Registration<ChangeWrapWidth>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .Constructor(
                &Management::GenerateValue<
                &PullFromParameter<0, Arca::RelicID>,
                &PullFromParameter<1, float>>,
                { "Arca::RelicID id", "float to" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::id>("Arca::RelicID", "id")
            .Property<&Type::to>("float", "to")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&ToArca>(engine, documentationManager);
    }

    Render::ChangeTextCore Registration<ChangeWrapWidth>::ToArca(Type fromAngelScript)
    {
        return Render::ChangeTextCore{ fromAngelScript.id, {}, {}, fromAngelScript.to, {}, {} };
    }

    void Registration<ChangeBold>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .Constructor(
                &Management::GenerateValue<
                &PullFromParameter<0, Arca::RelicID>,
                &PullFromParameter<1, bool>>,
                { "Arca::RelicID id", "bool to" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::id>("Arca::RelicID", "id")
            .Property<&Type::to>("bool", "to")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&ToArca>(engine, documentationManager);
    }

    Render::ChangeTextCore Registration<ChangeBold>::ToArca(Type fromAngelScript)
    {
        return Render::ChangeTextCore{ fromAngelScript.id, {}, {}, {}, fromAngelScript.to, {} };
    }

    void Registration<ChangeItalics>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .Constructor(
                &Management::GenerateValue<
                &PullFromParameter<0, Arca::RelicID>,
                &PullFromParameter<1, bool>>,
                { "Arca::RelicID id", "bool to" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::id>("Arca::RelicID", "id")
            .Property<&Type::to>("bool", "to")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&ToArca>(engine, documentationManager);
    }

    Render::ChangeTextCore Registration<ChangeItalics>::ToArca(Type fromAngelScript)
    {
        return Render::ChangeTextCore{ fromAngelScript.id, {}, {}, {}, {}, fromAngelScript.to };
    }
}