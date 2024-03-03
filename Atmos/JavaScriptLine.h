#pragma once

#include "JavaScriptType.h"
#include "Line.h"

#include "JavaScriptMaterialAsset.h"
#include "JavaScriptObject.h"

#include <Arca/Create.h>

#include <Inscription/OptionalScribe.h>

namespace Atmos::Scripting::JavaScript
{
    struct Line
    {
        Arca::RelicID id;
        std::vector<Spatial::Point2D> points;
        float z;
        std::optional<MaterialAsset> material;
        float width;
        Render::Color color;
    };

    template<>
    class Type<Line>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Line& value);
        [[nodiscard]] static std::optional<Line> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<std::vector<Spatial::Point2D>> points;
            Property<float> z;
            Property<std::optional<MaterialAsset>> material;
            Property<float> width;
            Property<Render::Color> color;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Index<Render::Line>>
    {
    public:
        using V8T = v8::Value;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Arca::Index<Render::Line>& value);
    };

    struct CreateLine
    {
        std::vector<Spatial::Point2D> points;
        float z;
        std::optional<MaterialAsset> material;
        float width;
        Render::Color color;
    };

    template<>
    class Type<CreateLine>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static std::optional<CreateLine> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<std::vector<Spatial::Point2D>> points;
            Property<float> z;
            Property<std::optional<MaterialAsset>> material;
            Property<float> width;
            Property<Render::Color> color;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Create<Render::Line>>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static std::optional<Arca::Create<Render::Line>> FromV8(
            v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Scripting::JavaScript::Line> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::Line;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            ScrivenRelicID("id", object.id, format);
            format("points", object.points);
            format("z", object.z);
            format("material", object.material);
            format("width", object.width);
            format("color", object.color);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::Line, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::Line>;
    };

    template<>
    class Scribe<Atmos::Scripting::JavaScript::CreateLine> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::CreateLine;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("points", object.points);
            format("z", object.z);
            format("material", object.material);
            format("width", object.width);
            format("color", object.color);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::CreateLine, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::CreateLine>;
    };
}