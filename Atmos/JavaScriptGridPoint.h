#pragma once

#include "JavaScriptType.h"
#include "GridPoint.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Spatial::Grid::Point>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Spatial::Grid::Point& value);
        [[nodiscard]] static std::optional<Spatial::Grid::Point> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Spatial::Grid::Point::Value> x;
            Property<Spatial::Grid::Point::Value> y;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}