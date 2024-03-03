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
                { "Atmos::Asset::Action action", "Atmos::Input::Key[]@ keys" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::action>("Atmos::Asset::Action", "action")
            .Property<&Type::keys>("Atmos::Input::Key[]", "keys")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&Chroma::Identity<Type>>(engine, documentationManager);
    }

    void Registration<Input::BindAction>::Constructor(asIScriptGeneric* generic)
    {
        const auto action = PullFromParameter<0, Arca::Index<Asset::Action>>(*generic);
        const auto keys = PullFromParameter<1, std::vector<Input::Key>>(*generic);

        const auto passKeys = Asset::Action::MappedKeys{ keys.begin(), keys.end() };
        Management::GenerateKnownValue(generic, action, passKeys);
    }
}