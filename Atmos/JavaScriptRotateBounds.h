#pragma once

#include "JavaScriptType.h"
#include "RotateBounds.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Spatial::RotateBounds>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Spatial::RotateBounds& value);
        [[nodiscard]] static std::optional<Spatial::RotateBounds> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<Spatial::Angle2D> to;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}