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

        static inline const String name = "Line";
        static inline const String containingNamespace = "Atmos::Render";
        static inline const String documentation = "This is a relic.";
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

        static inline const String name = "ChangeLinePoints";
        static inline const String containingNamespace = "Atmos::Render";
        static inline const String documentation = "This is a command.";
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);

        static Render::MoveLine ToArca(Type fromAngelScript);
    };

    template<>
    struct Registration<ChangeLineZ>
    {
        using Type = ChangeLineZ;

        static inline const String name = "ChangeLineZ";
        static inline const String containingNamespace = "Atmos::Render";
        static inline const String documentation = "This is a command.";
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);

        static Render::MoveLine ToArca(Type fromAngelScript);
    };
}