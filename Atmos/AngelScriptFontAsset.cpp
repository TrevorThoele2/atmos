#include "AngelScriptFontAsset.h"

#include "AngelScriptArcaTraits.h"
#include "AngelScriptArcaBatch.h"
#include "AngelScriptAsset.h"

#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Asset::Font>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type> registration(ContainingNamespace(), Name());
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&Management::Method<&Size>, "Atmos::Spatial::Size2D", "Size", { "string value" })
            .Actualize(engine, documentationManager);

        Registration<ArcaTraits<Asset::Font>>::RegisterTo(engine, documentationManager);
        Registration<Arca::Batch<Asset::Font>>::RegisterTo(engine, documentationManager);
        Registration<Asset::FindByName<Asset::Font>>::RegisterTo(engine, documentationManager);
    }

    Spatial::Size2D Registration<Asset::Font>::Size(Type type, String string)
    {
        return RequiredValue(type)->Size(string);
    }
}