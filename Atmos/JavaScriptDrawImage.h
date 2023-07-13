#pragma once

#include "JavaScriptType.h"
#include "DrawImage.h"

#include "JavaScriptImageAsset.h"
#include "JavaScriptShaders.h"
#include "JavaScriptSize2D.h"
#include "JavaScriptPoint2D.h"
#include "JavaScriptScalers2D.h"
#include "JavaScriptAxisAlignedBox2D.h"
#include "JavaScriptColor.h"
#include "JavaScriptNumeric.h"
#include "JavaScriptOptional.h"
#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Render::Raster::DrawImage>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Render::Raster::DrawImage& value);
        [[nodiscard]] static std::optional<Render::Raster::DrawImage> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Render::Shaders> shaders;
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