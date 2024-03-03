#include "AngelScriptPositionedSound.h"

#include "AngelScriptAsset.h"
#include "AngelScriptHandle.h"
#include "AngelScriptArcaTraits.h"
#include "AngelScriptArcaBatch.h"
#include "AngelScriptArcaCreate.h"
#include "AngelScriptArcaDestroy.h"

#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Audio::PositionedSound>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type> registration(ContainingNamespace(), Name());
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&Management::Method<&Asset>, "Atmos::Asset::Audio", "Asset", {})
            .ConstMethod(&Management::Method<&Volume>, "float", "Volume", {})
            .Actualize(engine, documentationManager);

        Registration<ArcaTraits<Audio::PositionedSound>>::RegisterTo(engine, documentationManager);
        Registration<Arca::Batch<Audio::PositionedSound>>::RegisterTo(engine, documentationManager);

        ArcaCreateRelicRegistration<Type::ValueT>()
            .Constructor<
                Arca::Index<Asset::Audio>,
                Audio::Volume,
                Spatial::Point3D>
            ({
                "Atmos::Asset::Audio asset",
                "float volume",
                "Atmos::Spatial::Point3D position"
            })
            .Actualize(engine, documentationManager);
        RegisterArcaCreated<Type::ValueT>(engine, documentationManager);

        RegisterArcaDestroyRelic<Type::ValueT>(engine, documentationManager);
        RegisterArcaDestroying<Type::ValueT>(engine, documentationManager);
    }

    Arca::Index<Asset::Audio> Registration<Audio::PositionedSound>::Asset(Type type)
    {
        return RequiredValue(type)->Asset();
    }

    Audio::Volume Registration<Audio::PositionedSound>::Volume(Type type)
    {
        return RequiredValue(type)->Volume();
    }
}