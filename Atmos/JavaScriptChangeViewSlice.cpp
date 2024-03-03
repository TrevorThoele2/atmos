#include "JavaScriptChangeViewSlice.h"

#include "JavaScriptNumeric.h"
#include "JavaScriptAxisAlignedBox2D.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Render::ChangeViewSlice>::ToV8(v8::Isolate& isolate, const Render::ChangeViewSlice& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.to.SetValue(isolate, value.to);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Render::ChangeViewSlice> Type<Render::ChangeViewSlice>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Render::ChangeViewSlice, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                const auto to = definition.to.AtmosValue();
                return id && to
                    ? Render::ChangeViewSlice
                    {
                        *id, *to
                    }
                    : std::optional<Render::ChangeViewSlice>{};
            });
    }

    Type< Render::ChangeViewSlice>::Definition::Definition() :
        id("id"),
        to("to")
    {}

    AnyObjectDefinition Type<Render::ChangeViewSlice>::Definition::ToAny()
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