#pragma once

#include "JavaScriptType.h"
#include "ShaderAsset.h"

#include "JavaScriptObject.h"
#include "JavaScriptRelicID.h"

namespace Atmos::Scripting::JavaScript
{
    struct ShaderAsset
    {
        Arca::RelicID id;
        String name;
    };

    template<>
    class Type<ShaderAsset>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const ShaderAsset& value);
        [[nodiscard]] static std::optional<ShaderAsset> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<String> name;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Index<Asset::Shader>>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Arca::Index<Asset::Shader>& value);
        [[nodiscard]] static std::optional<Arca::Index<Asset::Shader>> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Scripting::JavaScript::ShaderAsset> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::ShaderAsset;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            ScrivenRelicID("id", object.id, format);
            format("name", object.name);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::ShaderAsset, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::ShaderAsset>;
    };
}