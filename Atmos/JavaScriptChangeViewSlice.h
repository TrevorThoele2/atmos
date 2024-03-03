#pragma once

#include "JavaScriptType.h"
#include "ChangeViewSlice.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Render::ChangeViewSlice>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Render::ChangeViewSlice& value);
        [[nodiscard]] static std::optional<Render::ChangeViewSlice> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<Spatial::AxisAlignedBox2D> to;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}