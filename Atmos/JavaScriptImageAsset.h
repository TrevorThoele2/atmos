#pragma once

#include "JavaScriptType.h"
#include "ImageAsset.h"

#include "JavaScriptObject.h"
#include "JavaScriptRelicID.h"

namespace Atmos::Scripting::JavaScript
{
    struct ImageAsset
    {
        Arca::RelicID id;
        String name;
        Spatial::Size2D size;
        Asset::ImageGridSize gridSize;
        Spatial::Size2D sliceSize;
    };

    template<>
    class Type<ImageAsset>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const ImageAsset& value);
        [[nodiscard]] static std::optional<ImageAsset> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<String> name;
            Property<Spatial::Size2D> size;
            Property<Asset::ImageGridSize> gridSize;
            Property<Spatial::Size2D> sliceSize;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Index<Asset::Image>>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Arca::Index<Asset::Image>& value);
        [[nodiscard]] static std::optional<Arca::Index<Asset::Image>> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Scripting::JavaScript::ImageAsset> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::ImageAsset;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            ScrivenRelicID("id", object.id, format);
            format("name", object.name);
            format("size", object.size);
            format("gridSize", object.gridSize);
            format("sliceSize", object.sliceSize);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::ImageAsset, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::ImageAsset>;
    };
}