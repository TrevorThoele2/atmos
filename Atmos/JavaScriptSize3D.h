#pragma once

#include "JavaScriptType.h"
#include "Size3D.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Spatial::Size3D>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Spatial::Size3D& value);
        [[nodiscard]] static std::optional<Spatial::Size3D> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Spatial::Size3D::Value> width;
            Property<Spatial::Size3D::Value> height;
            Property<Spatial::Size3D::Value> depth;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}