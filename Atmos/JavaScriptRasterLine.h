#pragma once

#include "JavaScriptType.h"
#include "RasterLine.h"

#include "JavaScriptMaterialAsset.h"
#include "JavaScriptColor.h"
#include "JavaScriptPoint2D.h"
#include "JavaScriptNumeric.h"
#include "JavaScriptVector.h"
#include "JavaScriptOptional.h"
#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Render::Raster::Line>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Render::Raster::Line& value);
        [[nodiscard]] static std::optional<Render::Raster::Line> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<std::optional<MaterialAsset>> material;
            Property<std::vector<Spatial::Point2D>> points;
            Property<Render::LineWidth> width;
            Property<Render::Color> color;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}