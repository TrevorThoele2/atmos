#include "AngelScriptUniversalSound.h"

#include "AngelScriptAsset.h"
#include "AngelScriptHandle.h"
#include "AngelScriptArcaTraits.h"
#include "AngelScriptArcaBatch.h"
#include "AngelScriptArcaCreate.h"
#include "AngelScriptArcaDestroy.h"

#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Audio::UniversalSound>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type> registration(ContainingNamespace(), Name());
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&Management::Method<&Asset>, "Atmos::Asset::Audio", "Asset", {})
            .ConstMethod(&Management::Method<&Volume>, "float", "Volume", {})
            .Actualize(engine, documentationManager);

        Registration<ArcaTraits<Audio::UniversalSound>>::RegisterTo(engine, documentationManager);
        Registration<Arca::Batch<Audio::UniversalSound>>::RegisterTo(engine, documentationManager);

        ArcaCreateRelicRegistration<Type::ValueT>()
            .Constructor<
                Arca::Index<Asset::Audio>,
                Audio::Volume>
            ({
                "Atmos::Asset::Audio asset",
                "float volume"
            })
            .Actualize(engine, documentationManager);
        RegisterArcaCreated<Type::ValueT>(engine, documentationManager);

        RegisterArcaDestroyRelic<Type::ValueT>(engine, documentationManager);
        RegisterArcaDestroying<Type::ValueT>(engine, documentationManager);
    }

    Arca::Index<Asset::Audio> Registration<Audio::UniversalSound>::Asset(Type type)
    {
        return RequiredValue(type)->Asset();
    }

    Audio::Volume Registration<Audio::UniversalSound>::Volume(Type type)
    {
        return RequiredValue(type)->Volume();
    }
}