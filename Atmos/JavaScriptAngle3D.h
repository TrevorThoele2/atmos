#pragma once

#include "JavaScriptType.h"
#include "Angle3D.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Spatial::Angle3D>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Spatial::Angle3D& value);
        [[nodiscard]] static std::optional<Spatial::Angle3D> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Spatial::Angle3D::Value> yaw;
            Property<Spatial::Angle3D::Value> pitch;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}