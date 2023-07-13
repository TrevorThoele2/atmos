#pragma once

#include "JavaScriptType.h"
#include "DrawLine.h"

#include "JavaScriptShaders.h"
#include "JavaScriptColor.h"
#include "JavaScriptPoint2D.h"
#include "JavaScriptNumeric.h"
#include "JavaScriptVector.h"
#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Render::Raster::DrawLine>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Render::Raster::DrawLine& value);
        [[nodiscard]] static std::optional<Render::Raster::DrawLine> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Render::Shaders> shaders;
            Property<std::vector<Spatial::Point2D>> points;
            Property<Render::LineWidth> width;
            Property<Render::Color> color;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}