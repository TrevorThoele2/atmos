#pragma once

#include "JavaScriptType.h"
#include "DrawRegion.h"

#include "JavaScriptShaders.h"
#include "JavaScriptMaterialAsset.h"
#include "JavaScriptRenderMesh.h"
#include "JavaScriptNumeric.h"
#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Render::Raster::DrawRegion>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Render::Raster::DrawRegion& value);
        [[nodiscard]] static std::optional<Render::Raster::DrawRegion> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Render::Shaders> shaders;
            Property<Render::Mesh> mesh;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}