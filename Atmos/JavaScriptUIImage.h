#pragma once

#include "JavaScriptType.h"
#include "UIImage.h"

#include "JavaScriptImageAsset.h"
#include "JavaScriptMaterialAsset.h"
#include "JavaScriptObject.h"

#include <Arca/Create.h>

#include <Inscription/OptionalScribe.h>

namespace Atmos::Scripting::JavaScript
{
    struct UIImage
    {
        Arca::RelicID id;
        std::optional<ImageAsset> asset;
        Render::ImageCore::Index assetIndex;
        std::optional<MaterialAsset> material;
        Render::Color color;
        Spatial::Point3D position;
        Spatial::Scalers2D scalers;
        Spatial::Angle2D rotation;
    };

    template<>
    class Type<UIImage>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const UIImage& value);
        [[nodiscard]] static std::optional<UIImage> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<std::optional<ImageAsset>> asset;
            Property<Render::ImageCore::Index> assetIndex;
            Property<std::optional<MaterialAsset>> material;
            Property<Render::Color> color;
            Property<Spatial::Point3D> position;
            Property<Spatial::Scalers2D> scalers;
            Property<Spatial::Angle2D> rotation;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Index<UI::Image>>
    {
    public:
        using V8T = v8::Value;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Arca::Index<UI::Image>& value);
    };

    struct CreateUIImage
    {
        std::optional<ImageAsset> asset;
        Render::ImageCore::Index assetIndex;
        std::optional<MaterialAsset> material;
        Render::Color color;
        Spatial::Point3D position;
        Spatial::Scalers2D scalers;
        Spatial::Angle2D rotation;
    };

    template<>
    class Type<CreateUIImage>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static std::optional<CreateUIImage> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<std::optional<ImageAsset>> asset;
            Property<Render::ImageCore::Index> assetIndex;
            Property<std::optional<MaterialAsset>> material;
            Property<Render::Color> color;
            Property<Spatial::Point3D> position;
            Property<Spatial::Scalers2D> scalers;
            Property<Spatial::Angle2D> rotation;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Create<UI::Image>>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static std::optional<Arca::Create<UI::Image>> FromV8(
            v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Scripting::JavaScript::UIImage> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::UIImage;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            ScrivenRelicID("id", object.id, format);
            format("asset", object.asset);
            format("assetIndex", object.assetIndex);
            format("material", object.material);
            format("color", object.color);
            format("position", object.position);
            format("scalers", object.scalers);
            format("rotation", object.rotation);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::UIImage, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::UIImage>;
    };

    template<>
    class Scribe<Atmos::Scripting::JavaScript::CreateUIImage> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::CreateUIImage;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("asset", object.asset);
            format("assetIndex", object.assetIndex);
            format("material", object.material);
            format("color", object.color);
            format("position", object.position);
            format("scalers", object.scalers);
            format("rotation", object.rotation);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::CreateUIImage, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::CreateUIImage>;
    };
}