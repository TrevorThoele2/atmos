#pragma once

#include "JavaScriptType.h"
#include "RestartSound.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Audio::RestartSound>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Audio::RestartSound& value);
        [[nodiscard]] static std::optional<Audio::RestartSound> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}