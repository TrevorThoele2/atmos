#include "AngelScriptAudioAsset.h"

#include "AngelScriptArcaTraits.h"
#include "AngelScriptArcaBatch.h"
#include "AngelScriptAsset.h"

#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Asset::Audio>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type> registration(ContainingNamespace(), Name());
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&Management::Method<&DoName>, "string", "Name", {})
            .Actualize(engine, documentationManager);

        Registration<ArcaTraits<Asset::Audio>>::RegisterTo(engine, documentationManager);
        Registration<Arca::Batch<Asset::Audio>>::RegisterTo(engine, documentationManager);
        Registration<Asset::FindByName<Asset::Audio>>::RegisterTo(engine, documentationManager);
    }

    String Registration<Asset::Audio>::DoName(Type type)
    {
        return RequiredValue(type)->Name();
    }
}