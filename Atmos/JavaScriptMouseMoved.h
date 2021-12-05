#pragma once

#include "JavaScriptType.h"
#include "MouseMoved.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Input::MouseMoved>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Input::MouseMoved& value);
        [[nodiscard]] static std::optional<Input::MouseMoved> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Spatial::Point2D> previous;
            Property<Spatial::Point2D> current;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}