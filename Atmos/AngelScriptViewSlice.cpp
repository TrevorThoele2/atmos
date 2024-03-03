#include "AngelScriptViewSlice.h"

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
    void Registration<Render::ViewSlice>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type> registration(ContainingNamespace(), Name());
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&Management::Method<&Box>, "Atmos::Spatial::AxisAlignedBox2D", "Box", {})
            .Actualize(engine, documentationManager);

        Registration<ArcaTraits<Render::ViewSlice>>::RegisterTo(engine, documentationManager);
        Registration<Arca::Batch<Render::ViewSlice>>::RegisterTo(engine, documentationManager);

        ArcaCreateShardRegistration<Type::ValueT>()
            .Constructor<>({})
            .Constructor<Spatial::AxisAlignedBox2D>
            ({
                "Atmos::Spatial::AxisAlignedBox2D box"
            })
            .Actualize(engine, documentationManager);
        RegisterArcaCreated<Type::ValueT>(engine, documentationManager);

        RegisterArcaDestroyShard<Type::ValueT>(engine, documentationManager);
    }

    Spatial::AxisAlignedBox2D Registration<Render::ViewSlice>::Box(Type type)
    {
        return RequiredValue(type)->box;
    }
}