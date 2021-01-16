#include "AngelScriptScript.h"

#include "AngelScriptScriptAsset.h"

#include "AngelScriptObjectRegistration.h"
#include "AngelScriptArcaTraits.h"
#include "AngelScriptArcaBatch.h"
#include "AngelScriptArcaCreate.h"
#include "AngelScriptArcaDestroy.h"
#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Script>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type> registration(ContainingNamespace(), Name());
        RegisterArcaIndex(registration);
        registration
            .Actualize(engine, documentationManager);

        Registration<ArcaTraits<Script>>::RegisterTo(engine, documentationManager);
        Registration<Arca::Batch<Script>>::RegisterTo(engine, documentationManager);

        ArcaCreateRelicRegistration<Type::ValueT>()
            .Constructor<
                Arca::Index<Asset::Script>,
                String,
                std::vector<Variant>>
            ({
                "Atmos::Asset::Script asset",
                "string executeName",
                "Atmos::Variant[]@ parameters"
            })
            .Actualize(engine, documentationManager);
        RegisterArcaCreated<Type::ValueT>(engine, documentationManager);
        
        RegisterArcaDestroyRelic<Type::ValueT>(engine, documentationManager);
        RegisterArcaDestroying<Type::ValueT>(engine, documentationManager);
    }
}