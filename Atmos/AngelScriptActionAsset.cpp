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
        ValueTypeRegistration<Type> registration(containingNamespace, name);
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&Management::Method<&Name>, "string", "Name", {})
            .ConstMethod(&Management::Method<&MappedKeys>, "Atmos::Input::Key[]@", "MappedKeys", {})
            .Actualize(engine, documentationManager);

        Registration<ArcaTraits<Asset::Action>>::RegisterTo(engine, documentationManager);
        Registration<Arca::Batch<Asset::Action>>::RegisterTo(engine, documentationManager);
        Registration<Asset::FindByName<Asset::Action>>::RegisterTo(engine, documentationManager);
    }

    String Registration<Asset::Action>::Name(Type type)
    {
        return RequiredValue(type)->Name();
    }

    std::vector<Input::Key> Registration<Asset::Action>::MappedKeys(Type type)
    {
        const auto& mappedKeys = RequiredValue(type)->mappedKeys;
        return std::vector<Input::Key>{mappedKeys.begin(), mappedKeys.end()};
    }
}