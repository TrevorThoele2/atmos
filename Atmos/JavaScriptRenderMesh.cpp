#include "JavaScriptRenderMesh.h"

#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Render::Mesh>::ToV8(v8::Isolate& isolate, const Render::Mesh& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.vertices.SetValue(isolate, value.vertices);
        definition.indices.SetValue(isolate, value.indices);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Render::Mesh> Type<Render::Mesh>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Render::Mesh, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto vertices = definition.vertices.AtmosValue();
                const auto indices = definition.indices.AtmosValue();
                return vertices && indices
                    ? Render::Mesh
                    {
                        *vertices, *indices
                    }
                    : std::optional<Render::Mesh>{};
            });
    }

    Type<Render::Mesh>::Definition::Definition() :
        vertices("vertices"),
        indices("indices")
    {}

    AnyObjectDefinition Type<Render::Mesh>::Definition::ToAny()
    {
        return
        {
            {
                { vertices },
                { indices }
            }
        };
    }
}