#pragma once

#include "JavaScriptType.h"
#include "RenderMesh.h"

#include "JavaScriptVector.h"
#include "JavaScriptPoint2D.h"
#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Render::Mesh>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Render::Mesh& value);
        [[nodiscard]] static std::optional<Render::Mesh> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<std::vector<Spatial::Point2D>> vertices;
            Property<std::vector<std::uint16_t>> indices;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}