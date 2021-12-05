#include "JavaScriptSoundFinished.h"

#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Audio::SoundFinished>::ToV8(v8::Isolate& isolate, const Audio::SoundFinished& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Audio::SoundFinished> Type<Audio::SoundFinished>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Audio::SoundFinished, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                return id
                    ? Audio::SoundFinished
                {
                    *id
                }
                : std::optional<Audio::SoundFinished>{};
            });
    }

    Type<Audio::SoundFinished>::Definition::Definition() :
        id("id")
    {}

    AnyObjectDefinition Type<Audio::SoundFinished>::Definition::ToAny()
    {
        return
        {
            {
                { id }
            }
        };
    }
}