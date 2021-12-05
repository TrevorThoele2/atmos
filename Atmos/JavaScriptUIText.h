#pragma once

#include "JavaScriptType.h"
#include "UIText.h"

#include "JavaScriptFontAsset.h"
#include "JavaScriptMaterialAsset.h"
#include "JavaScriptObject.h"

#include <Arca/Create.h>

#include <Inscription/OptionalScribe.h>

namespace Atmos::Scripting::JavaScript
{
    struct UIText
    {
        Arca::RelicID id;
        String string;
        std::optional<FontAsset> asset;
        std::optional<MaterialAsset> material;
        float wrapWidth;
        bool bold;
        bool italics;
        Render::Color color;
        Spatial::Point3D position;
        Spatial::Scalers2D scalers;
        Spatial::Angle2D rotation;
    };

    template<>
    class Type<UIText>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const UIText& value);
        [[nodiscard]] static std::optional<UIText> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<String> string;
            Property<std::optional<FontAsset>> asset;
            Property<std::optional<MaterialAsset>> material;
            Property<float> wrapWidth;
            Property<bool> bold;
            Property<bool> italics;
            Property<Render::Color> color;
            Property<Spatial::Point3D> position;
            Property<Spatial::Scalers2D> scalers;
            Property<Spatial::Angle2D> rotation;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Index<UI::Text>>
    {
    public:
        using V8T = v8::Value;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Arca::Index<UI::Text>& value);
    };

    struct CreateUIText
    {
        String string;
        std::optional<FontAsset> asset;
        std::optional<MaterialAsset> material;
        float wrapWidth;
        bool bold;
        bool italics;
        Render::Color color;
        Spatial::Point3D position;
        Spatial::Scalers2D scalers;
        Spatial::Angle2D rotation;
    };

    template<>
    class Type<CreateUIText>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static std::optional<CreateUIText> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<String> string;
            Property<std::optional<FontAsset>> asset;
            Property<std::optional<MaterialAsset>> material;
            Property<float> wrapWidth;
            Property<bool> bold;
            Property<bool> italics;
            Property<Render::Color> color;
            Property<Spatial::Point3D> position;
            Property<Spatial::Scalers2D> scalers;
            Property<Spatial::Angle2D> rotation;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Create<UI::Text>>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static std::optional<Arca::Create<UI::Text>> FromV8(
            v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Scripting::JavaScript::UIText> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::UIText;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            ScrivenRelicID("id", object.id, format);
            format("string", object.string);
            format("asset", object.asset);
            format("material", object.material);
            format("wrapWidth", object.wrapWidth);
            format("bold", object.bold);
            format("italics", object.italics);
            format("color", object.color);
            format("position", object.position);
            format("scalers", object.scalers);
            format("rotation", object.rotation);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::UIText, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::UIText>;
    };

    template<>
    class Scribe<Atmos::Scripting::JavaScript::CreateUIText> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::CreateUIText;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("string", object.string);
            format("asset", object.asset);
            format("material", object.material);
            format("wrapWidth", object.wrapWidth);
            format("bold", object.bold);
            format("italics", object.italics);
            format("color", object.color);
            format("position", object.position);
            format("scalers", object.scalers);
            format("rotation", object.rotation);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::CreateUIText, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::CreateUIText>;
    };
}