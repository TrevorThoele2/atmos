#pragma once

#include "JavaScriptType.h"
#include "Key.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Input::Key>
    {
    public:
        using V8T = v8::Number;

        [[nodiscard]] static v8::Local<v8::Value> Register(const RegistrationData& data);

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Input::Key& value);
        [[nodiscard]] static std::optional<Input::Key> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };
}