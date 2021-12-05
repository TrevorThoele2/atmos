#include "JavaScriptFindImagesByBox.h"

#include "JavaScriptSpace.h"
#include "JavaScriptAxisAlignedBox3D.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Render::FindImagesByBox>::ToV8(v8::Isolate& isolate, const Render::FindImagesByBox& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.box.SetValue(isolate, value.box);
        definition.space.SetValue(isolate, value.space);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Render::FindImagesByBox> Type<Render::FindImagesByBox>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Render::FindImagesByBox, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto box = definition.box.AtmosValue();
                const auto space = definition.space.AtmosValue();
                return box && space
                    ? Render::FindImagesByBox
                    {
                        *box, *space
                    }
                    : std::optional<Render::FindImagesByBox>{};
            });
    }

    Type< Render::FindImagesByBox>::Definition::Definition() :
        box("box"),
        space("space")
    {}

    AnyObjectDefinition Type<Render::FindImagesByBox>::Definition::ToAny()
    {
        return
        {
            {
                { box },
                { space }
            }
        };
    }
}