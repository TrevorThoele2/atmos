#include "AngelScriptDiagnosticsStatistics.h"

#include "AngelScriptObjectRegistration.h"
#include "AngelScriptArcaTraits.h"
#include "AngelScriptArcaBatch.h"
#include "AngelScriptArcaCreate.h"
#include "AngelScriptArcaDestroy.h"
#include <angelscript.h>

#include "AngelScriptGridPoint.h"

namespace Atmos::Scripting::Angel
{
    void Registration<Diagnostics::Statistics>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type> registration(ContainingNamespace(), Name());
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&Management::Method<&RelicCount>, "uint32", "RelicCount", {})
            .ConstMethod(&Management::Method<&ShardCount>, "uint32", "ShardCount", {})
            .ConstMethod(&Management::Method<&NextRelicID>, "Arca::RelicID", "NextRelicID", {})
            .ConstMethod(&Management::Method<&RenderTime>, "double", "RenderTime", {})
            .ConstMethod(&Management::Method<&IdleTime>, "double", "IdleTime", {})
            .Actualize(engine, documentationManager);

        Registration<ArcaTraits<Diagnostics::Statistics>>::RegisterTo(engine, documentationManager);
    }

    size_t Registration<Diagnostics::Statistics>::RelicCount(Type type)
    {
        return RequiredValue(type)->relicCount;
    }

    size_t Registration<Diagnostics::Statistics>::ShardCount(Type type)
    {
        return RequiredValue(type)->shardCount;
    }

    Arca::RelicID Registration<Diagnostics::Statistics>::NextRelicID(Type type)
    {
        return RequiredValue(type)->nextRelicID;
    }

    double Registration<Diagnostics::Statistics>::RenderTime(Type type)
    {
        return RequiredValue(type)->renderTime;
    }

    double Registration<Diagnostics::Statistics>::IdleTime(Type type)
    {
        return RequiredValue(type)->idleTime;
    }
}
