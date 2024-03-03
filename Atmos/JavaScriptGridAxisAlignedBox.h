#pragma once

#include "JavaScriptType.h"
#include "GridAxisAlignedBox.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Spatial::Grid::AxisAlignedBox>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Spatial::Grid::AxisAlignedBox& value);
        [[nodiscard]] static std::optional<Spatial::Grid::AxisAlignedBox> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Spatial::Grid::Point> center;
            Property<Spatial::Grid::Size> size;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}