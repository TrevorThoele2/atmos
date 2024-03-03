#pragma once

#include "GridRegion.h"
#include "MoveGridRegion.h"

#include "AngelScriptMaterialAsset.h"

#include "AngelScriptIndex.h"
#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    class ChangeGridRegionPoints
    {
    public:
        Arca::RelicID id = 0;

        std::vector<Spatial::Grid::Point> to;
    };

    class ChangeGridRegionZ
    {
    public:
        Arca::RelicID id = 0;

        Spatial::Grid::Point::Value to;
    };

    template<>
    struct Registration<Render::GridRegion>
    {
        using Type = Arca::Index<Render::GridRegion>;
        using Management = ObjectManagement<Type>;

        static inline const String name = "GridRegion";
        static inline const String containingNamespace = Namespaces::Atmos::Render::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    private:
        [[nodiscard]] static std::unordered_set<Spatial::Grid::Point> Points(Type type);
        [[nodiscard]] static Spatial::Grid::Point::Value Z(Type type);
        [[nodiscard]] static Arca::Index<Asset::RegionMaterial> Material(Type type);
    };

    template<>
    struct Registration<Arca::Index<Render::GridRegion>> : Registration<Render::GridRegion>
    {};

    template<>
    struct Registration<ChangeGridRegionPoints>
    {
        using Type = ChangeGridRegionPoints;

        static inline const String name = "ChangeGridRegionPoints";
        static inline const String containingNamespace = Namespaces::Atmos::Render::name;
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine);

        static Render::MoveGridRegion ToArca(Type fromAngelScript);
    };

    template<>
    struct Registration<ChangeGridRegionZ>
    {
        using Type = ChangeGridRegionZ;

        static inline const String name = "ChangeGridRegionZ";
        static inline const String containingNamespace = Namespaces::Atmos::Render::name;
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine);

        static Render::MoveGridRegion ToArca(Type fromAngelScript);
    };
}