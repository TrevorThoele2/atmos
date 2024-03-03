#include "JavaScriptMaterialPass.h"

#include "JavaScriptShaderAsset.h"
#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<MaterialAssetPass>::ToV8(v8::Isolate& isolate, const MaterialAssetPass& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.vertexShader.SetValue(isolate, value.vertexShader);
        definition.fragmentShader.SetValue(isolate, value.fragmentShader);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<MaterialAssetPass> Type<MaterialAssetPass>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<MaterialAssetPass, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto vertexShader = definition.vertexShader.AtmosValue();
                const auto fragmentShader = definition.fragmentShader.AtmosValue();
                return vertexShader && fragmentShader
                    ? MaterialAssetPass
                    {
                        *vertexShader, *fragmentShader
                    }
                    : std::optional<MaterialAssetPass>{};
            });
    }

    Type<MaterialAssetPass>::Definition::Definition() :
        vertexShader("vertexShader"),
        fragmentShader("fragmentShader")
    {}

    AnyObjectDefinition Type<MaterialAssetPass>::Definition::ToAny()
    {
        return
        {
            {
                { vertexShader },
                { fragmentShader }
            }
        };
    }

    auto Type<Asset::MaterialPass>::ToV8(v8::Isolate& isolate, const Asset::MaterialPass& value) -> v8::Local<V8T>
    {
        return JavaScript::ToV8(isolate, value);
    }
}