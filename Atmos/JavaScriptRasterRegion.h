#pragma once

#include "JavaScriptType.h"
#include "RasterRegion.h"

#include "JavaScriptMaterialAsset.h"
#include "JavaScriptRenderMesh.h"
#include "JavaScriptNumeric.h"
#include "JavaScriptOptional.h"
#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Render::Raster::Region>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Render::Raster::Region& value);
        [[nodiscard]] static std::optional<Render::Raster::Region> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<std::optional<MaterialAsset>> material;
            Property<Render::Mesh> mesh;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}