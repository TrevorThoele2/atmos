#pragma once

#include "JavaScriptType.h"
#include "Color.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Render::Color>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Render::Color& value);
        [[nodiscard]] static std::optional<Render::Color> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Render::Color::Value> alpha;
            Property<Render::Color::Value> red;
            Property<Render::Color::Value> green;
            Property<Render::Color::Value> blue;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}