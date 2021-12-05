#pragma once

#include "JavaScriptType.h"
#include "Scalers2D.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Spatial::Scalers2D>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Spatial::Scalers2D& value);
        [[nodiscard]] static std::optional<Spatial::Scalers2D> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Spatial::Scalers2D::Value> x;
            Property<Spatial::Scalers2D::Value> y;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}