#pragma once

#include "JavaScriptType.h"
#include "DrawText.h"

#include "JavaScriptShaders.h"
#include "JavaScriptFontAsset.h"
#include "JavaScriptRenderMesh.h"
#include "JavaScriptNumeric.h"
#include "JavaScriptOptional.h"
#include "JavaScriptAxisAlignedBox2D.h"
#include "JavaScriptColor.h"
#include "JavaScriptBool.h"
#include "JavaScriptScalers2D.h"
#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Render::Raster::DrawText>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Render::Raster::DrawText& value);
        [[nodiscard]] static std::optional<Render::Raster::DrawText> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Render::Shaders> shaders;
            Property<String> string;
            Property<std::optional<FontAsset>> font;
            Property<Spatial::AxisAlignedBox2D> viewSlice;
            Property<Render::Color> color;
            Property<bool> bold;
            Property<bool> italics;
            Property<float> wrapWidth;

            Property<Spatial::Point2D> position;
            Property<Spatial::Angle2D> rotation;
            Property<Spatial::Scalers2D> scalers;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}