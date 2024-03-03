#include "AngelScriptRenderCore.h"

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
    void Registration<Render::RenderCore>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type> registration(ContainingNamespace(), Name());
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&Management::Method<&Material>, "Atmos::Asset::Material", "Material", {})
            .ConstMethod(&Management::Method<&Color>, "Atmos::Render::Color", "Color", {})
            .Actualize(engine, documentationManager);

        Registration<ArcaTraits<Render::RenderCore>>::RegisterTo(engine, documentationManager);
        Registration<Arca::Batch<Render::RenderCore>>::RegisterTo(engine, documentationManager);

        ArcaCreateShardRegistration<Type::ValueT>()
            .Constructor<>({})
            .Constructor<
                Arca::Index<Asset::Material>,
                Render::Color>
            ({
                "Atmos::Asset::Material material",
                "Atmos::Render::Color color"
            })
            .Actualize(engine, documentationManager);
        RegisterArcaCreated<Type::ValueT>(engine, documentationManager);

        RegisterArcaDestroyShard<Type::ValueT>(engine, documentationManager);
    }

    Arca::Index<Asset::Material> Registration<Render::RenderCore>::Material(Type type)
    {
        return RequiredValue(type)->material;
    }

    Render::Color Registration<Render::RenderCore>::Color(Type type)
    {
        return RequiredValue(type)->color;
    }
}