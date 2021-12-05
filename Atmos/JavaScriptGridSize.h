#pragma once

#include "JavaScriptType.h"
#include "GridSize.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Spatial::Grid::Size>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Spatial::Grid::Size& value);
        [[nodiscard]] static std::optional<Spatial::Grid::Size> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Spatial::Grid::Size::Value> width;
            Property<Spatial::Grid::Size::Value> height;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}