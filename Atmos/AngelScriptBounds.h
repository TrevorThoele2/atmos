#pragma once

#include "Bounds.h"
#include "MoveBounds.h"
#include "RotateBounds.h"
#include "ScaleBounds.h"
#include "BoundsMoved.h"
#include "BoundsRotated.h"
#include "BoundsScaled.h"

#include "AngelScriptIndex.h"
#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Spatial::BoundsSpace>
    {
        static String Name() { return "BoundsSpace"; }
        static String ContainingNamespace() { return "Atmos::Spatial"; }
        static String Documentation() { return "Represents what space a Bounds is in."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<>
    struct Registration<Spatial::Bounds>
    {
        using Type = Arca::Index<Spatial::Bounds>;
        using Management = ObjectManagement<Type>;

        static String Name() { return "Bounds"; }
        static String ContainingNamespace() { return "Atmos::Spatial"; }
        static String Documentation() { return "This is a shard."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static Spatial::Point3D Position(Type type);
        [[nodiscard]] static Spatial::Size2D BaseSize(Type type);
        [[nodiscard]] static Spatial::Size2D Size(Type type);
        [[nodiscard]] static Spatial::Scalers2D Scalers(Type type);
        [[nodiscard]] static Spatial::Angle2D Rotation(Type type);
        [[nodiscard]] static Spatial::BoundsSpace Space(Type type);
    };

    template<>
    struct Registration<Arca::Index<Spatial::Bounds>> : Registration<Spatial::Bounds>
    {};

    template<>
    struct Registration<Spatial::MoveBounds>
    {
        using Type = Spatial::MoveBounds;
        using Management = ObjectManagement<Type>;

        static String Name() { return "MoveBounds"; }
        static String ContainingNamespace() { return "Atmos::Spatial"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<>
    struct Registration<Spatial::RotateBounds>
    {
        using Type = Spatial::RotateBounds;
        using Management = ObjectManagement<Type>;

        static String Name() { return "RotateBounds"; }
        static String ContainingNamespace() { return "Atmos::Spatial"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<>
    struct Registration<Spatial::ScaleBounds>
    {
        using Type = Spatial::ScaleBounds;
        using Management = ObjectManagement<Type>;

        static String Name() { return "ScaleBounds"; }
        static String ContainingNamespace() { return "Atmos::Spatial"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<>
    struct Registration<Spatial::BoundsMoved>
    {
        using Type = Spatial::BoundsMoved;
        using Management = ObjectManagement<Type>;

        static String Name() { return "BoundsMoved"; }
        static String ContainingNamespace() { return "Atmos::Spatial"; }
        static String Documentation() { return "This is a signal."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<>
    struct Registration<Spatial::BoundsRotated>
    {
        using Type = Spatial::BoundsRotated;
        using Management = ObjectManagement<Type>;

        static String Name() { return "BoundsRotated"; }
        static String ContainingNamespace() { return "Atmos::Spatial"; }
        static String Documentation() { return "This is a signal."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<>
    struct Registration<Spatial::BoundsScaled>
    {
        using Type = Spatial::BoundsScaled;
        using Management = ObjectManagement<Type>;

        static String Name() { return "BoundsScaled"; }
        static String ContainingNamespace() { return "Atmos::Spatial"; }
        static String Documentation() { return "This is a signal."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}