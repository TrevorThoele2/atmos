#include "JavaScriptCamera.h"

#include "JavaScriptPoint3D.h"
#include "JavaScriptSize2D.h"
#include "JavaScriptAxisAlignedBox2D.h"
#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Camera>::ToV8(v8::Isolate& isolate, const Camera& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.position.SetValue(isolate, value.position);
        definition.size.SetValue(isolate, value.size);
        definition.sides.SetValue(isolate, value.sides);
        return CreateObject(isolate, definition.ToAny());
    }

    Type<Camera>::Definition::Definition() :
        id("id"),
        position("position"),
        size("size"),
        sides("sides")
    {}

    AnyObjectDefinition Type<Camera>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { position },
                { size },
                { sides }
            }
        };
    }

    auto Type<Arca::Index<Render::Camera>>::ToV8(v8::Isolate& isolate, const Arca::Index<Render::Camera>& value) -> v8::Local<V8T>
    {
        if (!value)
            return v8::Null(&isolate);

        return JavaScript::ToV8(
            isolate,
            Camera
            {
                value.ID(),
                value->Position(),
                value->Size(),
                value->Sides()
            });
    }
}