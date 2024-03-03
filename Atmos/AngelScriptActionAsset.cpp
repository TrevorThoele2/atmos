#include "AngelScriptActionAsset.h"

#include "AngelScriptArcaTraits.h"
#include "AngelScriptArcaBatch.h"
#include "AngelScriptAsset.h"
#include "AngelScriptKey.h"

#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Asset::Action>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type> registration(ContainingNamespace(), Name());
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&Management::Method<&DoName>, "string", "Name", {})
            .ConstMethod(&Management::Method<&DoBoundKey>, "Atmos::Input::Key", "BoundKey", {})
            .ConstMethod(&Management::Method<&DoBoundModifiers>, "Atmos::Input::Key[]@", "BoundModifiers", {})
            .Actualize(engine, documentationManager);

        Registration<ArcaTraits<Asset::Action>>::RegisterTo(engine, documentationManager);
        Registration<Arca::Batch<Asset::Action>>::RegisterTo(engine, documentationManager);
        Registration<Asset::FindByName<Asset::Action>>::RegisterTo(engine, documentationManager);
    }

    String Registration<Asset::Action>::DoName(Type type)
    {
        return RequiredValue(type)->Name();
    }

    Input::Key Registration<Asset::Action>::DoBoundKey(Type type)
    {
        return RequiredValue(type)->boundKey;
    }

    std::vector<Input::Key> Registration<Asset::Action>::DoBoundModifiers(Type type)
    {
        const auto& boundModifiers = RequiredValue(type)->boundModifiers;
        return std::vector<Input::Key>{boundModifiers.begin(), boundModifiers.end()};
    }
}