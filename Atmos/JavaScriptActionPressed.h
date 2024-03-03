#pragma once

#include "JavaScriptType.h"
#include "ActionPressed.h"

#include "JavaScriptActionAsset.h"
#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Input::ActionPressed>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Input::ActionPressed& value);
        [[nodiscard]] static std::optional<Input::ActionPressed> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<std::optional<ActionAsset>> action;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}