#pragma once

#include "Line.h"
#include "MoveLine.h"

#include "AngelScriptMaterialAsset.h"

#include "AngelScriptIndex.h"
#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    class ChangeLinePoints
    {
    public:
        Arca::RelicID id = 0;

        std::vector<Spatial::Point2D> to;
    };

    class ChangeLineZ
    {
    public:
        Arca::RelicID id = 0;

        Spatial::Point2D::Value to;
    };

    template<>
    struct Registration<Render::Line>
    {
        using Type = Arca::Index<Render::Line>;
        using Management = ObjectManagement<Type>;

        static String Name() { return "Line"; }
        static String ContainingNamespace() { return "Atmos::Render"; }
        static String Documentation() { return "This is a relic."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static std::vector<Spatial::Point2D> Points(Type type);
        [[nodiscard]] static Spatial::Point2D::Value Z(Type type);
        [[nodiscard]] static Arca::Index<Asset::LineMaterial> Material(Type type);
        [[nodiscard]] static Render::LineWidth Width(Type type);
        [[nodiscard]] static Render::Color Color(Type type);
    };

    template<>
    struct Registration<Arca::Index<Render::Line>> : Registration<Render::Line>
    {};

    template<>
    struct Registration<ChangeLinePoints>
    {
        using Type = ChangeLinePoints;

        static String Name() { return "ChangeLinePoints"; }
        static String ContainingNamespace() { return "Atmos::Render"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);

        static Render::MoveLine ToArca(Type fromAngelScript);
    };

    template<>
    struct Registration<ChangeLineZ>
    {
        using Type = ChangeLineZ;

        static String Name() { return "ChangeLineZ"; }
        static String ContainingNamespace() { return "Atmos::Render"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);

        static Render::MoveLine ToArca(Type fromAngelScript);
    };
}