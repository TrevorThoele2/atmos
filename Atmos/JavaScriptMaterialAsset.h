#pragma once

#include "JavaScriptType.h"
#include "MaterialAsset.h"

#include "JavaScriptObject.h"
#include "JavaScriptRelicID.h"
#include "JavaScriptScriptAsset.h"

#include <Inscription/OptionalScribe.h>

namespace Atmos::Scripting::JavaScript
{
    struct MaterialAsset
    {
        Arca::RelicID id;
        String name;
        std::optional<ScriptAsset> asset;
        String executeName;
        Parameters parameters;
    };

    template<>
    class Type<MaterialAsset>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const MaterialAsset& value);
        [[nodiscard]] static std::optional<MaterialAsset> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<String> name;
            Property<std::optional<ScriptAsset>> asset;
            Property<String> executeName;
            Property<Parameters> parameters;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Index<Asset::Material>>
    {
    public:
        using V8T = v8::Value;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Arca::Index<Asset::Material>& value);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Scripting::JavaScript::MaterialAsset> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::MaterialAsset;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            ScrivenRelicID("id", object.id, format);
            format("name", object.name);
            format("asset", object.asset);
            format("executeName", object.executeName);
            format("parameters", object.parameters);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::MaterialAsset, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::MaterialAsset>;
    };
}