#pragma once

#include "JavaScriptType.h"
#include "ActionDepressed.h"

#include "JavaScriptActionAsset.h"
#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Input::ActionDepressed>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Input::ActionDepressed& value);
        [[nodiscard]] static std::optional<Input::ActionDepressed> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<std::optional<ActionAsset>> action;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}