#pragma once

#include "JavaScriptType.h"
#include "Point2D.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Spatial::Point2D>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Spatial::Point2D& value);
        [[nodiscard]] static std::optional<Spatial::Point2D> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Spatial::Point2D::Value> x;
            Property<Spatial::Point2D::Value> y;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}