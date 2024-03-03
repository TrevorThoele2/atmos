#include "JavaScriptMaterialAsset.h"

#include "JavaScriptMaterialPass.h"
#include "JavaScriptVector.h"
#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<MaterialAsset>::ToV8(v8::Isolate& isolate, const MaterialAsset& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.name.SetValue(isolate, value.name);
        definition.passes.SetValue(isolate, value.passes);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<MaterialAsset> Type<MaterialAsset>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<MaterialAsset, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                const auto name = definition.name.AtmosValue();
                const auto passes = definition.passes.AtmosValue();
                return id && name && passes
                    ? MaterialAsset
                    {
                        *id, *name, *passes
                    }
                    : std::optional<MaterialAsset>{};
            });
    }

    Type<MaterialAsset>::Definition::Definition() :
        id("id"),
        name("name"),
        passes("passes")
    {}

    AnyObjectDefinition Type<MaterialAsset>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { name },
                { passes }
            }
        };
    }

    auto Type<Arca::Index<Asset::Material>>::ToV8(v8::Isolate& isolate, const Arca::Index<Asset::Material>& value) -> v8::Local<V8T>
    {
        std::vector<MaterialAssetPass> passes;
        passes.reserve(value->Passes().size());
        for (auto& pass : value->Passes())
        {
            const auto vertex = pass.VertexShader();
            const auto fragment = pass.FragmentShader();
            passes.emplace_back(ShaderAsset{ vertex.ID(), vertex->Name() }, ShaderAsset{ fragment.ID(), fragment->Name() });
        }
        return JavaScript::ToV8(
            isolate,
            MaterialAsset
            {
                value.ID(),
                value->Name(),
                passes
            });
    }

    std::optional<Arca::Index<Asset::Material>> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto userData = static_cast<UserData*>(isolate.GetData(0));

        const auto from = JavaScript::FromV8<MaterialAsset>(isolate, value);
        return from ? userData->reliquary->Find<Asset::Material>(from->id) : std::optional<Arca::Index<Asset::Material>>{};
    }
}