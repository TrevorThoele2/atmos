#pragma once

#include "Camera.h"

#include "AngelScriptIndex.h"
#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Render::Camera>
    {
        using Type = Arca::Index<Render::Camera>;
        using Management = ObjectManagement<Type>;

        static inline const String name = "Camera";
        static inline const String containingNamespace = "Atmos::Render";
        static inline const String documentation = "This is a relic. Contains a Bounds.";
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static Spatial::Point3D Position(Type type);
        [[nodiscard]] static Spatial::Size2D Size(Type type);

        [[nodiscard]] static Spatial::AxisAlignedBox2D Sides(Type type);
    };

    template<>
    struct Registration<Arca::Index<Render::Camera>> : Registration<Render::Camera>
    {};
}