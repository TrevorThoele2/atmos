#pragma once

#include "JavaScriptType.h"
#include "TextBaseSize.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Render::TextBaseSize>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Render::TextBaseSize& value);
        [[nodiscard]] static std::optional<Render::TextBaseSize> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<String> string;
            Property<Arca::RelicID> font;
            Property<float> wrapWidth;
            Property<bool> bold;
            Property<bool> italics;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}