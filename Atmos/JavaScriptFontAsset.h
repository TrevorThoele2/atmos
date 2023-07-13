#pragma once

#include "JavaScriptType.h"
#include "FontAsset.h"

#include "JavaScriptObject.h"
#include "JavaScriptRelicID.h"

namespace Atmos::Scripting::JavaScript
{
    struct FontAsset
    {
        Arca::RelicID id;
        String name;
    };

    template<>
    class Type<FontAsset>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const FontAsset& value);
        [[nodiscard]] static std::optional<FontAsset> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
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
    class Type<Arca::Index<Asset::Font>>
    {
    public:
        using V8T = v8::Value;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Arca::Index<Asset::Font>& value);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Scripting::JavaScript::FontAsset> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::FontAsset;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            ScrivenRelicID("id", object.id, format);
            format("name", object.name);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::FontAsset, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::FontAsset>;
    };
}