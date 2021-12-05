#include "JavaScriptViewSlice.h"

#include "JavaScriptNumeric.h"
#include "JavaScriptAxisAlignedBox2D.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<ViewSlice>::ToV8(v8::Isolate& isolate, const ViewSlice& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.box.SetValue(isolate, value.box);
        return CreateObject(isolate, definition.ToAny());
    }

    Type<ViewSlice>::Definition::Definition() :
        id("id"),
        box("box")
    {}

    AnyObjectDefinition Type<ViewSlice>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { box }
            }
        };
    }

    auto Type<Arca::Index<Render::ViewSlice>>::ToV8(v8::Isolate& isolate, const Arca::Index<Render::ViewSlice>& value) -> v8::Local<V8T>
    {
        if (!value)
            return v8::Null(&isolate);

        return JavaScript::ToV8(
            isolate,
            ViewSlice
            {
                value.ID(),
                value->box
            });
    }

    std::optional<CreateViewSlice> Type<CreateViewSlice>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<CreateViewSlice, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                const auto box = definition.box.AtmosValue();
                return id && box
                    ? CreateViewSlice
                    {
                        *id, *box
                    }
                    : std::optional<CreateViewSlice>{};
            });
    }

    Type<CreateViewSlice>::Definition::Definition() :
        id("id"),
        box("box")
    {}

    AnyObjectDefinition Type<CreateViewSlice>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { box }
            }
        };
    }

    std::optional<Arca::Create<Render::ViewSlice>> Type<Arca::Create<Render::ViewSlice>>::FromV8(
        v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto userData = static_cast<UserData*>(isolate.GetData(0));
        const auto reliquary = userData->reliquary;

        const auto command = JavaScript::FromV8<CreateViewSlice>(isolate, value);
        return command
            ? Arca::Create<Render::ViewSlice>(
                command->id,
                command->box)
            : std::optional<Arca::Create<Render::ViewSlice>>{};
    }
}