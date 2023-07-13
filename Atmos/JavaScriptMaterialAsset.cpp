#include "JavaScriptMaterialAsset.h"

#include "JavaScriptVector.h"
#include "JavaScriptNumeric.h"
#include "JavaScriptOptional.h"
#include "JavaScriptVariant.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<MaterialAsset>::ToV8(v8::Isolate& isolate, const MaterialAsset& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.name.SetValue(isolate, value.name);
        definition.asset.SetValue(isolate, value.asset);
        definition.executeName.SetValue(isolate, value.executeName);
        definition.parameters.SetValue(isolate, value.parameters);
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
                const auto asset = definition.asset.AtmosValue();
                const auto executeName = definition.executeName.AtmosValue();
                const auto parameters = definition.parameters.AtmosValue();
                return id && name && asset && executeName && parameters
                    ? MaterialAsset
                    {
                        *id, *name, *asset, *executeName, *parameters
                    }
                    : std::optional<MaterialAsset>{};
            });
    }

    Type<MaterialAsset>::Definition::Definition() :
        id("id"),
        name("name"),
        asset("asset"),
        executeName("executeName"),
        parameters("parameters")
    {}

    AnyObjectDefinition Type<MaterialAsset>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { name },
                { asset },
                { executeName },
                { parameters }
            }
        };
    }

    auto Type<Arca::Index<Asset::Material>>::ToV8(v8::Isolate& isolate, const Arca::Index<Asset::Material>& value) -> v8::Local<V8T>
    {
        if (!value)
            return v8::Null(&isolate);

        const auto asset = value->scriptData.asset;

        return JavaScript::ToV8(
            isolate,
            MaterialAsset
            {
                value.ID(),
                value->Name(),
                asset
                    ? ScriptAsset
                    {
                        asset.ID(),
                        asset->Name()
                    }
                    : std::optional<ScriptAsset>(),
                value->scriptData.executeName,
                value->scriptData.parameters
            });
    }

    std::optional<Arca::Index<Asset::Material>> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto userData = static_cast<UserData*>(isolate.GetData(0));

        const auto from = JavaScript::FromV8<MaterialAsset>(isolate, value);
        return from ? userData->reliquary->Find<Asset::Material>(from->id) : std::optional<Arca::Index<Asset::Material>>{};
    }
}