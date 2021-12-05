#pragma once

#include "JavaScriptType.h"
#include "ToWorldPoint3D.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Spatial::ToWorldPoint3D>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Spatial::ToWorldPoint3D& value);
        [[nodiscard]] static std::optional<Spatial::ToWorldPoint3D> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Spatial::Point2D> from;
            Property<Spatial::Point2D::Value> z;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}