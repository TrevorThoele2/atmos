#pragma once

#include "JavaScriptType.h"
#include "AxisAlignedBox3D.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Spatial::AxisAlignedBox3D>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Spatial::AxisAlignedBox3D& value);
        [[nodiscard]] static std::optional<Spatial::AxisAlignedBox3D> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Spatial::Point3D> center;
            Property<Spatial::Size3D> size;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}