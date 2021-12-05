#pragma once

#include "JavaScriptType.h"
#include "AxisAlignedBox2D.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Spatial::AxisAlignedBox2D>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Spatial::AxisAlignedBox2D& value);
        [[nodiscard]] static std::optional<Spatial::AxisAlignedBox2D> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Spatial::Point2D> center;
            Property<Spatial::Size2D> size;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}