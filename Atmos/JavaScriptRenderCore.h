#pragma once

#include "JavaScriptType.h"
#include "RenderCore.h"

#include "JavaScriptMaterialAsset.h"

#include "JavaScriptObject.h"
#include "JavaScriptRelicID.h"
#include <Arca/ShardIndex.h>
#include <Arca/RelicID.h>
#include <Arca/Create.h>

namespace Atmos::Scripting::JavaScript
{
    struct RenderCore
    {
        Arca::RelicID id;
        std::optional<MaterialAsset> material;
        Render::Color color;
    };

    template<>
    class Type<RenderCore>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const RenderCore& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<std::optional<MaterialAsset>> material;
            Property<Render::Color> color;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Index<Render::RenderCore>>
    {
    public:
        using V8T = v8::Value;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Arca::Index<Render::RenderCore>& value);
    };

    struct CreateRenderCore
    {
        Arca::RelicID id = 0;
        std::optional<MaterialAsset> material;
        Render::Color color;
    };

    template<>
    class Type<CreateRenderCore>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static std::optional<CreateRenderCore> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<std::optional<MaterialAsset>> material;
            Property<Render::Color> color;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Create<Render::RenderCore>>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static std::optional<Arca::Create<Render::RenderCore>> FromV8(
            v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Scripting::JavaScript::RenderCore> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::RenderCore;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            ScrivenRelicID("id", object.id, format);
            format("material", object.material);
            format("color", object.color);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::RenderCore, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::RenderCore>;
    };

    template<>
    class Scribe<Atmos::Scripting::JavaScript::CreateRenderCore> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::CreateRenderCore;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            ScrivenRelicID("id", object.id, format);
            format("material", object.material);
            format("color", object.color);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::CreateRenderCore, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::CreateRenderCore>;
    };
}