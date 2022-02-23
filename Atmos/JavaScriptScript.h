#pragma once

#include "JavaScriptType.h"
#include "Script.h"

#include "JavaScriptScriptAsset.h"
#include "JavaScriptObject.h"
#include "JavaScriptVariant.h"

#include <Arca/Create.h>

#include <Inscription/OptionalScribe.h>

namespace Atmos::Scripting::JavaScript
{
    struct Script
    {
        Arca::RelicID id;
        std::optional<ScriptAsset> asset;
        String executeName;
        Parameters parameters;
    };

    template<>
    class Type<Script>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Script& value);
        [[nodiscard]] static std::optional<Script> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<std::optional<ScriptAsset>> asset;
            Property<String> executeName;
            Property<Parameters> parameters;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Index<Scripting::Script>>
    {
    public:
        using V8T = v8::Value;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Arca::Index<Scripting::Script>& value);
    };

    struct CreateScript
    {
        std::optional<ScriptAsset> asset;
        String executeName;
        Parameters parameters;
    };

    template<>
    class Type<CreateScript>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static std::optional<CreateScript> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<std::optional<ScriptAsset>> asset;
            Property<String> executeName;
            Property<Parameters> parameters;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Create<Scripting::Script>>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static std::optional<Arca::Create<Scripting::Script>> FromV8(
            v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Scripting::JavaScript::Script> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::Script;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            ScrivenRelicID("id", object.id, format);
            format("asset", object.asset);
            format("executeName", object.executeName);
            format("parameters", object.parameters);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::Script, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::Script>;
    };

    template<>
    class Scribe<Atmos::Scripting::JavaScript::CreateScript> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::CreateScript;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("asset", object.asset);
            format("executeName", object.executeName);
            format("parameters", object.parameters);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::CreateScript, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::CreateScript>;
    };
}