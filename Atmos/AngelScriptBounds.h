#pragma once

#include "Bounds.h"
#include "MoveBoundsBy.h"
#include "MoveBoundsDirection.h"
#include "MoveBoundsTo.h"
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
    struct Registration<Spatial::Bounds>
    {
        using Type = Arca::Index<Spatial::Bounds>;
        using Management = ObjectManagement<Type>;

        static inline const String name = "Bounds";
        static inline const String containingNamespace = Namespaces::Atmos::Spatial::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    private:
        [[nodiscard]] static Spatial::Point3D Position(Type type);
        [[nodiscard]] static Spatial::Size2D BaseSize(Type type);
        [[nodiscard]] static Spatial::Size2D Size(Type type);
        [[nodiscard]] static Spatial::Scalers2D Scalers(Type type);
        [[nodiscard]] static Spatial::Angle2D Rotation(Type type);
    };

    template<>
    struct Registration<Arca::Index<Spatial::Bounds>> : Registration<Spatial::Bounds>
    {};

    template<>
    struct Registration<Spatial::MoveBoundsBy>
    {
        using Type = Spatial::MoveBoundsBy;
        using Management = ObjectManagement<Type>;

        static inline const String name = "MoveBoundsBy";
        static inline const String containingNamespace = Namespaces::Atmos::Spatial::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    };

    template<>
    struct Registration<Spatial::MoveBoundsDirection>
    {
        using Type = Spatial::MoveBoundsDirection;
        using Management = ObjectManagement<Type>;

        static inline const String name = "MoveBoundsDirection";
        static inline const String containingNamespace = Namespaces::Atmos::Spatial::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    };

    template<>
    struct Registration<Spatial::MoveBoundsTo>
    {
        using Type = Spatial::MoveBoundsTo;
        using Management = ObjectManagement<Type>;

        static inline const String name = "MoveBoundsTo";
        static inline const String containingNamespace = Namespaces::Atmos::Spatial::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    };

    template<>
    struct Registration<Spatial::RotateBounds>
    {
        using Type = Spatial::RotateBounds;
        using Management = ObjectManagement<Type>;

        static inline const String name = "RotateBounds";
        static inline const String containingNamespace = Namespaces::Atmos::Spatial::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    };

    template<>
    struct Registration<Spatial::ScaleBounds>
    {
        using Type = Spatial::ScaleBounds;
        using Management = ObjectManagement<Type>;

        static inline const String name = "ScaleBounds";
        static inline const String containingNamespace = Namespaces::Atmos::Spatial::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    };

    template<>
    struct Registration<Spatial::BoundsMoved>
    {
        using Type = Spatial::BoundsMoved;
        using Management = ObjectManagement<Type>;

        static inline const String name = "BoundsMoved";
        static inline const String containingNamespace = Namespaces::Atmos::Spatial::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    };

    template<>
    struct Registration<Spatial::BoundsRotated>
    {
        using Type = Spatial::BoundsRotated;
        using Management = ObjectManagement<Type>;

        static inline const String name = "BoundsRotated";
        static inline const String containingNamespace = Namespaces::Atmos::Spatial::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    };

    template<>
    struct Registration<Spatial::BoundsScaled>
    {
        using Type = Spatial::BoundsScaled;
        using Management = ObjectManagement<Type>;

        static inline const String name = "BoundsScaled";
        static inline const String containingNamespace = Namespaces::Atmos::Spatial::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    };
}