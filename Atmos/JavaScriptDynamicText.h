#pragma once

#include "JavaScriptType.h"
#include "DynamicText.h"

#include "JavaScriptFontAsset.h"
#include "JavaScriptMaterialAsset.h"
#include "JavaScriptObject.h"

#include <Arca/Create.h>

#include <Inscription/OptionalScribe.h>

namespace Atmos::Scripting::JavaScript
{
    struct DynamicText
    {
        Arca::RelicID id;
        String string;
        std::optional<FontAsset> font;
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
    class Type<DynamicText>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const DynamicText& value);
        [[nodiscard]] static std::optional<DynamicText> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<String> string;
            Property<std::optional<FontAsset>> font;
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
    class Type<Arca::Index<Render::DynamicText>>
    {
    public:
        using V8T = v8::Value;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Arca::Index<Render::DynamicText>& value);
    };

    struct CreateDynamicText
    {
        String string;
        std::optional<FontAsset> font;
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
    class Type<CreateDynamicText>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static std::optional<CreateDynamicText> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<String> string;
            Property<std::optional<FontAsset>> font;
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
    class Type<Arca::Create<Render::DynamicText>>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static std::optional<Arca::Create<Render::DynamicText>> FromV8(
            v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Scripting::JavaScript::DynamicText> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::DynamicText;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            ScrivenRelicID("id", object.id, format);
            format("string", object.string);
            format("font", object.font);
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
    struct ScribeTraits<Atmos::Scripting::JavaScript::DynamicText, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::DynamicText>;
    };

    template<>
    class Scribe<Atmos::Scripting::JavaScript::CreateDynamicText> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::CreateDynamicText;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("string", object.string);
            format("font", object.font);
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
    struct ScribeTraits<Atmos::Scripting::JavaScript::CreateDynamicText, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::CreateDynamicText>;
    };
}