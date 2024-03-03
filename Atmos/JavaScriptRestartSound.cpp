#include "JavaScriptRestartSound.h"

#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Audio::RestartSound>::ToV8(v8::Isolate& isolate, const Audio::RestartSound& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Audio::RestartSound> Type<Audio::RestartSound>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Audio::RestartSound, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                return id
                    ? Audio::RestartSound
                    {
                        *id
                    }
                    : std::optional<Audio::RestartSound>{};
            });
    }

    Type<Audio::RestartSound>::Definition::Definition() :
        id("id")
    {}

    AnyObjectDefinition Type<Audio::RestartSound>::Definition::ToAny()
    {
        return
        {
            {
                { id }
            }
        };
    }
}