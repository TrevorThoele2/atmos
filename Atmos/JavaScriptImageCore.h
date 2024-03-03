#pragma once

#include "JavaScriptType.h"
#include "ImageCore.h"

#include "JavaScriptImageAsset.h"

#include "JavaScriptObject.h"
#include "JavaScriptRelicID.h"
#include <Arca/ShardIndex.h>
#include <Arca/RelicID.h>
#include <Arca/Create.h>

#include <Inscription/OptionalScribe.h>

namespace Atmos::Scripting::JavaScript
{
    struct ImageCore
    {
        Arca::RelicID id;
        std::optional<ImageAsset> asset;
        Render::ImageCore::Index assetIndex;
    };

    template<>
    class Type<ImageCore>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const ImageCore& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<std::optional<ImageAsset>> asset;
            Property<Render::ImageCore::Index> assetIndex;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Index<Render::ImageCore>>
    {
    public:
        using V8T = v8::Value;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Arca::Index<Render::ImageCore>& value);
    };

    struct CreateImageCore
    {
        Arca::RelicID id = 0;
        std::optional<ImageAsset> asset;
        Render::ImageCore::Index assetIndex;
    };

    template<>
    class Type<CreateImageCore>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static std::optional<CreateImageCore> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<std::optional<ImageAsset>> asset;
            Property<Render::ImageCore::Index > assetIndex;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Create<Render::ImageCore>>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static std::optional<Arca::Create<Render::ImageCore>> FromV8(
            v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Scripting::JavaScript::ImageCore> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::ImageCore;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            ScrivenRelicID("id", object.id, format);
            format("asset", object.asset);
            format("assetIndex", object.assetIndex);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::ImageCore, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::ImageCore>;
    };

    template<>
    class Scribe<Atmos::Scripting::JavaScript::CreateImageCore> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::CreateImageCore;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            ScrivenRelicID("id", object.id, format);
            format("asset", object.asset);
            format("assetIndex", object.assetIndex);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::CreateImageCore, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::CreateImageCore>;
    };
}