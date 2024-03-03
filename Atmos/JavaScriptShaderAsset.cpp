#include "JavaScriptShaderAsset.h"

#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<ShaderAsset>::ToV8(v8::Isolate& isolate, const ShaderAsset& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.name.SetValue(isolate, value.name);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<ShaderAsset> Type<ShaderAsset>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<ShaderAsset, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                const auto name = definition.name.AtmosValue();
                return id && name
                    ? ShaderAsset
                    {
                        *id, *name
                    }
                    : std::optional<ShaderAsset>{};
            });
    }

    Type<ShaderAsset>::Definition::Definition() :
        id("id"),
        name("name")
    {}

    AnyObjectDefinition Type<ShaderAsset>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { name }
            }
        };
    }

    auto Type<Arca::Index<Asset::Shader>>::ToV8(v8::Isolate& isolate, const Arca::Index<Asset::Shader>& value) -> v8::Local<V8T>
    {
        return JavaScript::ToV8(
            isolate,
            ShaderAsset
            {
                value.ID(),
                value->Name()
            });
    }

    std::optional<Arca::Index<Asset::Shader>> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto userData = static_cast<UserData*>(isolate.GetData(0));

        const auto from = JavaScript::FromV8<ShaderAsset>(isolate, value);
        return from ? userData->reliquary->Find<Asset::Shader>(from->id) : std::optional<Arca::Index<Asset::Shader>>{};
    }
}