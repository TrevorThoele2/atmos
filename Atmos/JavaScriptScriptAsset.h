#pragma once

#include "JavaScriptType.h"
#include "ScriptAsset.h"

#include "JavaScriptObject.h"
#include "JavaScriptRelicID.h"

namespace Atmos::Scripting::JavaScript
{
    struct ScriptAsset
    {
        Arca::RelicID id;
        String name;
    };

    template<>
    class Type<ScriptAsset>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const ScriptAsset& value);
        [[nodiscard]] static std::optional<ScriptAsset> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
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
    class Type<Arca::Index<Asset::Script>>
    {
    public:
        using V8T = v8::Value;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Arca::Index<Asset::Script>& value);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Scripting::JavaScript::ScriptAsset> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::ScriptAsset;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            ScrivenRelicID("id", object.id, format);
            format("name", object.name);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::ScriptAsset, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::ScriptAsset>;
    };
}