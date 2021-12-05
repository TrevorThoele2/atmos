#include "JavaScriptPauseSound.h"

#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Audio::PauseSound>::ToV8(v8::Isolate& isolate, const Audio::PauseSound& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Audio::PauseSound> Type<Audio::PauseSound>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Audio::PauseSound, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                return id
                    ? Audio::PauseSound
                    {
                        *id
                    }
                    : std::optional<Audio::PauseSound>{};
            });
    }

    Type<Audio::PauseSound>::Definition::Definition() :
        id("id")
    {}

    AnyObjectDefinition Type<Audio::PauseSound>::Definition::ToAny()
    {
        return
        {
            {
                { id }
            }
        };
    }
}