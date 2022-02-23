#include "JavaScriptAudioAsset.h"

#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<AudioAsset>::ToV8(v8::Isolate& isolate, const AudioAsset& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.name.SetValue(isolate, value.name);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<AudioAsset> Type<AudioAsset>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<AudioAsset, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                const auto name = definition.name.AtmosValue();
                return id && name
                    ? AudioAsset
                    {
                        *id, *name
                    }
                    : std::optional<AudioAsset>{};
            });
    }

    Type<AudioAsset>::Definition::Definition() :
        id("id"),
        name("name")
    {}

    AnyObjectDefinition Type<AudioAsset>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { name }
            }
        };
    }

    auto Type<Arca::Index<Asset::Audio>>::ToV8(v8::Isolate& isolate, const Arca::Index<Asset::Audio>& value) -> v8::Local<V8T>
    {
        return JavaScript::ToV8(
            isolate,
            AudioAsset
            {
                value.ID(),
                value->Name()
            });
    }
}