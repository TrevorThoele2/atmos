#include "JavaScriptFindGridRegionsByBox.h"

#include "JavaScriptAxisAlignedBox3D.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Render::FindGridRegionsByBox>::ToV8(v8::Isolate& isolate, const Render::FindGridRegionsByBox& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.box.SetValue(isolate, value.box);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Render::FindGridRegionsByBox> Type<Render::FindGridRegionsByBox>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Render::FindGridRegionsByBox, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto box = definition.box.AtmosValue();
                return box
                    ? Render::FindGridRegionsByBox
                {
                    *box
                }
                : std::optional<Render::FindGridRegionsByBox>{};
            });
    }

    Type< Render::FindGridRegionsByBox>::Definition::Definition() :
        box("box")
    {}

    AnyObjectDefinition Type<Render::FindGridRegionsByBox>::Definition::ToAny()
    {
        return
        {
            {
                { box }
            }
        };
    }
}