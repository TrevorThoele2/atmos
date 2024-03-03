#include "AngelScriptCamera.h"

#include "AngelScriptHandle.h"
#include "AngelScriptArcaTraits.h"

#include "AngelScriptObjectRegistration.h"
#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Render::Camera>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type> registration(ContainingNamespace(), Name());
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&Management::Method<&Position>, "Atmos::Spatial::Point3D", "Position", {})
            .ConstMethod(&Management::Method<&Size>, "Atmos::Spatial::Size2D", "Size", {})
            .ConstMethod(&Management::Method<&Sides>, "Atmos::Spatial::AxisAlignedBox2D", "Sides", {})
            .Actualize(engine, documentationManager);

        Registration<ArcaTraits<Render::Camera>>::RegisterTo(engine, documentationManager);
    }

    Spatial::Point3D Registration<Render::Camera>::Position(Type type)
    {
        return RequiredValue(type)->Position();
    }

    Spatial::Size2D Registration<Render::Camera>::Size(Type type)
    {
        return RequiredValue(type)->Size();
    }

    Spatial::AxisAlignedBox2D Registration<Render::Camera>::Sides(Type type)
    {
        return RequiredValue(type)->Sides();
    }
}