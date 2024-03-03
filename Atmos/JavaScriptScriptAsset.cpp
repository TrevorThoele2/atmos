#include "JavaScriptScriptAsset.h"

#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<ScriptAsset>::ToV8(v8::Isolate& isolate, const ScriptAsset& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.name.SetValue(isolate, value.name);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<ScriptAsset> Type<ScriptAsset>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<ScriptAsset, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                const auto name = definition.name.AtmosValue();
                return id && name
                    ? ScriptAsset
                    {
                        *id, *name
                    }
                    : std::optional<ScriptAsset>{};
            });
    }

    Type<ScriptAsset>::Definition::Definition() :
        id("id"),
        name("name")
    {}

    AnyObjectDefinition Type<ScriptAsset>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { name }
            }
        };
    }

    auto Type<Arca::Index<Asset::Script>>::ToV8(v8::Isolate& isolate, const Arca::Index<Asset::Script>& value) -> v8::Local<V8T>
    {
        return JavaScript::ToV8(
            isolate,
            ScriptAsset
            {
                value.ID(),
                value->Name()
            });
    }

    std::optional<Arca::Index<Asset::Script>> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto userData = static_cast<UserData*>(isolate.GetData(0));

        const auto from = JavaScript::FromV8<ScriptAsset>(isolate, value);
        return from ? userData->reliquary->Find<Asset::Script>(from->id) : std::optional<Arca::Index<Asset::Script>>{};
    }
}