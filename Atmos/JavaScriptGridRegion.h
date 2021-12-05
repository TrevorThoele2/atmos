#pragma once

#include "JavaScriptType.h"
#include "GridRegion.h"

#include "JavaScriptImageAsset.h"
#include "JavaScriptMaterialAsset.h"
#include "JavaScriptObject.h"

#include <Arca/Create.h>

#include <Inscription/OptionalScribe.h>

namespace Atmos::Scripting::JavaScript
{
    struct GridRegion
    {
        Arca::RelicID id;
        std::unordered_set<Spatial::Grid::Point> points;
        int z;
        std::optional<MaterialAsset> material;
    };

    template<>
    class Type<GridRegion>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const GridRegion& value);
        [[nodiscard]] static std::optional<GridRegion> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<std::unordered_set<Spatial::Grid::Point>> points;
            Property<int> z;
            Property<std::optional<MaterialAsset>> material;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Index<Render::GridRegion>>
    {
    public:
        using V8T = v8::Value;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Arca::Index<Render::GridRegion>& value);
    };

    struct CreateGridRegion
    {
        std::unordered_set<Spatial::Grid::Point> points;
        int z;
        std::optional<MaterialAsset> material;
    };

    template<>
    class Type<CreateGridRegion>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static std::optional<CreateGridRegion> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<std::unordered_set<Spatial::Grid::Point>> points;
            Property<int> z;
            Property<std::optional<MaterialAsset>> material;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Create<Render::GridRegion>>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static std::optional<Arca::Create<Render::GridRegion>> FromV8(
            v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Scripting::JavaScript::GridRegion> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::GridRegion;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            ScrivenRelicID("id", object.id, format);
            format("points", object.points);
            format("z", object.z);
            format("material", object.material);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::GridRegion, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::GridRegion>;
    };

    template<>
    class Scribe<Atmos::Scripting::JavaScript::CreateGridRegion> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::CreateGridRegion;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("points", object.points);
            format("z", object.z);
            format("material", object.material);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::CreateGridRegion, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::CreateGridRegion>;
    };
}