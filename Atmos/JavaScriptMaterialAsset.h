#pragma once

#include "JavaScriptType.h"
#include "MaterialAsset.h"

#include "JavaScriptObject.h"
#include "JavaScriptRelicID.h"
#include "JavaScriptMaterialPass.h"

namespace Atmos::Scripting::JavaScript
{
    struct MaterialAsset
    {
        Arca::RelicID id;
        String name;
        std::vector<MaterialAssetPass> passes;
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
            Property<std::vector<MaterialAssetPass>> passes;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Index<Asset::Material>>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Arca::Index<Asset::Material>& value);
        [[nodiscard]] static std::optional<Arca::Index<Asset::Material>> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
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
            format("passes", object.passes);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::MaterialAsset, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::MaterialAsset>;
    };
}