#include "JavaScriptChangeColor.h"

#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Render::ChangeColor>::ToV8(v8::Isolate& isolate, const Render::ChangeColor& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.to.SetValue(isolate, value.to);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Render::ChangeColor> Type<Render::ChangeColor>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Render::ChangeColor, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                const auto to = definition.to.AtmosValue();
                return id && to
                    ? Render::ChangeColor
                    {
                        *id, *to
                    }
                    : std::optional<Render::ChangeColor>{};
            });
    }

    Type< Render::ChangeColor>::Definition::Definition() :
        id("id"),
        to("to")
    {}

    AnyObjectDefinition Type<Render::ChangeColor>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { to }
            }
        };
    }
}