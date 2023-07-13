#pragma once

#include "JavaScriptType.h"
#include "ActionAsset.h"

#include "JavaScriptObject.h"
#include "JavaScriptRelicID.h"

namespace Atmos::Scripting::JavaScript
{
    struct ActionAsset
    {
        Arca::RelicID id;
        String name;
        Input::Key boundKey;
        std::vector<Input::Key> boundModifiers;
    };

    template<>
    class Type<ActionAsset>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const ActionAsset& value);
        [[nodiscard]] static std::optional<ActionAsset> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<String> name;
            Property<Input::Key> boundKey;
            Property<std::vector<Input::Key>> boundModifiers;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Index<Asset::Action>>
    {
    public:
        using V8T = v8::Value;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Arca::Index<Asset::Action>& value);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Scripting::JavaScript::ActionAsset> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::ActionAsset;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            ScrivenRelicID("id", object.id, format);
            format("name", object.name);
            format("boundKey", object.boundKey);
            format("boundModifiers", object.boundModifiers);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::ActionAsset, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::ActionAsset>;
    };
}