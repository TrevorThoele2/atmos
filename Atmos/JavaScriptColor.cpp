#include "JavaScriptColor.h"

#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Render::Color>::ToV8(v8::Isolate& isolate, const Render::Color& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.alpha.SetValue(isolate, value.alpha);
        definition.red.SetValue(isolate, value.red);
        definition.green.SetValue(isolate, value.green);
        definition.blue.SetValue(isolate, value.blue);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Render::Color> Type<Render::Color>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Render::Color, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto alpha = definition.alpha.AtmosValue();
                const auto red = definition.red.AtmosValue();
                const auto green = definition.green.AtmosValue();
                const auto blue = definition.blue.AtmosValue();
                return alpha && red && green && blue
                    ? Render::Color
                    {
                        *alpha, *red, *green, *blue
                    }
                    : std::optional<Render::Color>{};
            });
    }

    Type<Render::Color>::Definition::Definition() :
        alpha("alpha"),
        red("red"),
        green("green"),
        blue("blue")
    {}

    AnyObjectDefinition Type<Render::Color>::Definition::ToAny()
    {
        return
        {
            {
                { alpha },
                { red },
                { green },
                { blue }
            }
        };
    }
}