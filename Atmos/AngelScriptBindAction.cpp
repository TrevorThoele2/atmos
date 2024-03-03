#include "AngelScriptBindAction.h"

#include "AngelScriptScript.h"
#include "AngelScriptCommand.h"
#include "AngelScriptObjectRegistration.h"

#include <angelscript.h>

#include <Chroma/Identity.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Input::BindAction>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .Constructor(
                &Constructor,
                { "Atmos::Asset::Action action", "Atmos::Input::Key key", "Atmos::Input::Key[]@ modifiers" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::action>("Atmos::Asset::Action", "action")
            .Property<&Type::key>("Atmos::Input::Key", "key")
            .Property<&Type::modifiers>("Atmos::Input::Key[]", "modifiers")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&Chroma::Identity<Type>>(engine, documentationManager);
    }

    void Registration<Input::BindAction>::Constructor(asIScriptGeneric* generic)
    {
        const auto action = PullFromParameter<0, Arca::Index<Asset::Action>>(*generic);
        const auto key = PullFromParameter<1, Input::Key>(*generic);
        const auto modifiers = PullFromParameter<2, std::vector<Input::Key>>(*generic);

        const auto passModifiers = Asset::Action::Modifiers{ modifiers.begin(), modifiers.end() };
        Management::GenerateKnownValue(generic, action, key, passModifiers);
    }
}