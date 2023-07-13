#include "JavaScriptScript.h"

#include "JavaScriptOptional.h"
#include "JavaScriptScriptAsset.h"
#include "JavaScriptNumeric.h"
#include "JavaScriptVector.h"
#include "JavaScriptString.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Script>::ToV8(v8::Isolate& isolate, const Script& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.asset.SetValue(isolate, value.asset);
        definition.executeName.SetValue(isolate, value.executeName);
        definition.parameters.SetValue(isolate, value.parameters);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Script> Type<Script>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Script, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                const auto asset = definition.asset.AtmosValue();
                const auto executeName = definition.executeName.AtmosValue();
                const auto parameters = definition.parameters.AtmosValue();
                return id && asset && executeName && parameters
                    ? Script
                    {
                        *id, *asset, *executeName, *parameters
                    }
                    : std::optional<Script>{};
            });
    }

    Type<Script>::Definition::Definition() :
        id("id"),
        asset("asset"),
        executeName("executeName"),
        parameters("parameters")
    {}

    AnyObjectDefinition Type<Script>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { asset },
                { executeName },
                { parameters }
            }
        };
    }

    auto Type<Arca::Index<Scripting::Script>>::ToV8(v8::Isolate& isolate, const Arca::Index<Scripting::Script>& value) -> v8::Local<V8T>
    {
        if (!value)
            return v8::Null(&isolate);

        const auto asset = value->data.asset;

        return JavaScript::ToV8(
            isolate,
            Script
            {
                value.ID(),
                asset
                    ? ScriptAsset
                    {
                        asset.ID(),
                        asset->Name()
                    }
                    : std::optional<ScriptAsset>(),
                value->data.executeName,
                value->data.parameters
            });
    }

    std::optional<CreateScript> Type<CreateScript>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<CreateScript, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto asset = definition.asset.AtmosValue();
                const auto executeName = definition.executeName.AtmosValue();
                const auto parameters = definition.parameters.AtmosValue();
                return asset && executeName && parameters
                    ? CreateScript
                    {
                        *asset, *executeName, *parameters
                    }
                    : std::optional<CreateScript>{};
            });
    }

    Type<CreateScript>::Definition::Definition() :
        asset("asset"),
        executeName("executeName"),
        parameters("parameters")
    {}

    AnyObjectDefinition Type<CreateScript>::Definition::ToAny()
    {
        return
        {
            {
                { asset },
                { executeName },
                { parameters }
            }
        };
    }

    std::optional<Arca::Create<Scripting::Script>> Type<Arca::Create<Scripting::Script>>::FromV8(
        v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto userData = static_cast<UserData*>(isolate.GetData(0));
        const auto reliquary = userData->reliquary;

        const auto command = JavaScript::FromV8<CreateScript>(isolate, value);
        return command
            ? Arca::Create<Scripting::Script>(
                command->asset ? reliquary->Find<Asset::Script>(command->asset->id) : Arca::Index<Asset::Script>(),
                command->executeName,
                command->parameters)
            : std::optional<Arca::Create<Scripting::Script>>{};
    }
}