#include "JavaScriptResumeSound.h"

#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Audio::ResumeSound>::ToV8(v8::Isolate& isolate, const Audio::ResumeSound& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Audio::ResumeSound> Type<Audio::ResumeSound>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Audio::ResumeSound, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                return id
                    ? Audio::ResumeSound
                {
                    *id
                }
                : std::optional<Audio::ResumeSound>{};
            });
    }

    Type<Audio::ResumeSound>::Definition::Definition() :
        id("id")
    {}

    AnyObjectDefinition Type<Audio::ResumeSound>::Definition::ToAny()
    {
        return
        {
            {
                { id }
            }
        };
    }
}