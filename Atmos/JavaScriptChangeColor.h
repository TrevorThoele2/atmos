#pragma once

#include "JavaScriptType.h"
#include "ChangeColor.h"

#include "JavaScriptColor.h"
#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Render::ChangeColor>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Render::ChangeColor& value);
        [[nodiscard]] static std::optional<Render::ChangeColor> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<Render::Color> to;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}