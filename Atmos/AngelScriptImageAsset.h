#pragma once

#include "ImageAsset.h"

#include "AngelScriptIndex.h"
#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Asset::ImageSize>
    {
        using Type = Asset::ImageSize;
        using Management = ObjectManagement<Type>;

        static inline const String name = "ImageSize";
        static inline const String containingNamespace = "Atmos::Asset";
        static inline const String documentation = "Represents an image asset's size.";
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<>
    struct Registration<Asset::ImageGridSize>
    {
        using Type = Asset::ImageGridSize;
        using Management = ObjectManagement<Type>;

        static inline const String name = "ImageGridSize";
        static inline const String containingNamespace = "Atmos::Asset";
        static inline const String documentation = "Represents an image asset's grid size (columns and rows).";
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<>
    struct Registration<Asset::Image>
    {
        using Type = Arca::Index<Asset::Image>;
        using Management = ObjectManagement<Type>;

        static inline const String name = "Image";
        static inline const String containingNamespace = "Atmos::Asset";
        static inline const String documentation = "This is a relic.";
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static String Name(Type type);

        [[nodiscard]] static Type::ValueT::Dimension Width(Type type);
        [[nodiscard]] static Type::ValueT::Dimension Height(Type type);
        [[nodiscard]] static Asset::ImageSize Size(Type type);

        [[nodiscard]] static Type::ValueT::GridDimension Columns(Type type);
        [[nodiscard]] static Type::ValueT::GridDimension Rows(Type type);
        [[nodiscard]] static Asset::ImageGridSize GridSize(Type type);

        [[nodiscard]] static Spatial::AxisAlignedBox2D Slice(Type type, int index);
        [[nodiscard]] static Spatial::Size2D SliceSize(Type type);
    };

    template<>
    struct Registration<Arca::Index<Asset::Image>> : Registration<Asset::Image>
    {};
}