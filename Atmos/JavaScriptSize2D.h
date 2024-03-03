#pragma once

#include "JavaScriptType.h"
#include "Size2D.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Spatial::Size2D>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Spatial::Size2D& value);
        [[nodiscard]] static std::optional<Spatial::Size2D> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Spatial::Size2D::Value> width;
            Property<Spatial::Size2D::Value> height;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}