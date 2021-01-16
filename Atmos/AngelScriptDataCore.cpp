#include "AngelScriptDataCore.h"

#include "AngelScriptProperty.h"

#include "AngelScriptObjectRegistration.h"
#include "AngelScriptCommand.h"
#include "AngelScriptHandle.h"
#include "AngelScriptArcaTraits.h"
#include "AngelScriptArcaBatch.h"
#include "AngelScriptArcaCreate.h"
#include "AngelScriptArcaDestroy.h"

#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    void Registration<DataCore>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type> registration(ContainingNamespace(), Name());
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&Management::Method<&Properties>, "Atmos::Property[]@", "Properties", {})
            .Actualize(engine, documentationManager);

        Registration<ArcaTraits<DataCore>>::RegisterTo(engine, documentationManager);
        Registration<Arca::Batch<DataCore>>::RegisterTo(engine, documentationManager);

        ArcaCreateShardRegistration<Type::ValueT>()
            .Constructor<>({})
            .Constructor<
                std::vector<Property>>
            ({
                "Atmos::Property[]@ properties"
            })
            .Actualize(engine, documentationManager);
        RegisterArcaCreated<Type::ValueT>(engine, documentationManager);

        RegisterArcaDestroyShard<Type::ValueT>(engine, documentationManager);
    }

    std::vector<Property> Registration<DataCore>::Properties(Type type)
    {
        return RequiredValue(type)->properties;
    }
}