#include "JavaScriptFindLinesByBox.h"

#include "JavaScriptAxisAlignedBox3D.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Render::FindLinesByBox>::ToV8(v8::Isolate& isolate, const Render::FindLinesByBox& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.box.SetValue(isolate, value.box);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Render::FindLinesByBox> Type<Render::FindLinesByBox>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Render::FindLinesByBox, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto box = definition.box.AtmosValue();
                return box
                    ? Render::FindLinesByBox
                    {
                        *box
                    }
                    : std::optional<Render::FindLinesByBox>{};
            });
    }

    Type< Render::FindLinesByBox>::Definition::Definition() :
        box("box")
    {}

    AnyObjectDefinition Type<Render::FindLinesByBox>::Definition::ToAny()
    {
        return
        {
            {
                { box }
            }
        };
    }
}