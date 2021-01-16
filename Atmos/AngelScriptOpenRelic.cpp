#include "AngelScriptOpenRelic.h"

#include "AngelScriptAsset.h"
#include "AngelScriptHandle.h"
#include "AngelScriptArcaTraits.h"
#include "AngelScriptArcaBatch.h"
#include "AngelScriptArcaCreate.h"
#include "AngelScriptArcaDestroy.h"

#include "AngelScriptObjectRegistration.h"
#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Arca::OpenRelic>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type> registration(ContainingNamespace(), Name());
        RegisterArcaIndex(registration);
        registration
            .Actualize(engine, documentationManager);

        Registration<ArcaTraits<Arca::OpenRelic>>::RegisterTo(engine, documentationManager);
        Registration<Arca::Batch<Arca::OpenRelic>>::RegisterTo(engine, documentationManager);

        ArcaCreateRelicRegistration<Type::ValueT>()
            .Constructor<>({})
            .Actualize(engine, documentationManager);
        RegisterArcaCreated<Type::ValueT>(engine, documentationManager);

        RegisterArcaDestroyRelic<Type::ValueT>(engine, documentationManager);
        RegisterArcaDestroying<Type::ValueT>(engine, documentationManager);
    }
}