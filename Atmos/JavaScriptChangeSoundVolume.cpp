#include "JavaScriptChangeSoundVolume.h"

#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Audio::ChangeSoundVolume>::ToV8(v8::Isolate& isolate, const Audio::ChangeSoundVolume& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.to.SetValue(isolate, value.to);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Audio::ChangeSoundVolume> Type<Audio::ChangeSoundVolume>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Audio::ChangeSoundVolume, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                const auto to = definition.to.AtmosValue();
                return id && to
                    ? Audio::ChangeSoundVolume
                    {
                        *id,
                        *to
                    }
                    : std::optional<Audio::ChangeSoundVolume>{};
            });
    }

    Type<Audio::ChangeSoundVolume>::Definition::Definition() :
        id("id"),
        to("to")
    {}

    AnyObjectDefinition Type<Audio::ChangeSoundVolume>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { to }
            }
        };
    }
}