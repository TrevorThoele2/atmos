#pragma once

#include "JavaScriptType.h"
#include "RasterImage.h"

#include "JavaScriptMaterialAsset.h"
#include "JavaScriptImageAsset.h"
#include "JavaScriptAxisAlignedBox2D.h"
#include "JavaScriptColor.h"
#include "JavaScriptPoint2D.h"
#include "JavaScriptSize2D.h"
#include "JavaScriptScalers2D.h"
#include "JavaScriptNumeric.h"
#include "JavaScriptOptional.h"
#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Render::Raster::Image>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Render::Raster::Image& value);
        [[nodiscard]] static std::optional<Render::Raster::Image> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<std::optional<MaterialAsset>> material;
            Property<std::optional<ImageAsset>> asset;
            Property<Spatial::AxisAlignedBox2D> assetSlice;
            Property<Spatial::AxisAlignedBox2D> viewSlice;
            Property<Render::Color> color;

            Property<Spatial::Point2D> position;
            Property<Spatial::Size2D> size;
            Property<Spatial::Angle2D> rotation;
            Property<Spatial::Scalers2D> scalers;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}