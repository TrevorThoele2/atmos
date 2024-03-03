#pragma once

#include "JavaScriptType.h"
#include "Scalers3D.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Spatial::Scalers3D>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Spatial::Scalers3D& value);
        [[nodiscard]] static std::optional<Spatial::Scalers3D> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Spatial::Scalers3D::Value> x;
            Property<Spatial::Scalers3D::Value> y;
            Property<Spatial::Scalers3D::Value> z;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}