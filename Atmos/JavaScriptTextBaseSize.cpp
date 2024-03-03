#include "JavaScriptTextBaseSize.h"

#include "JavaScriptBool.h"
#include "JavaScriptNumeric.h"
#include "JavaScriptString.h"
#include "JavaScriptOptional.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Render::TextBaseSize>::ToV8(v8::Isolate& isolate, const Render::TextBaseSize& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.string.SetValue(isolate, value.string);
        definition.font.SetValue(isolate, value.font);
        definition.wrapWidth.SetValue(isolate, value.wrapWidth);
        definition.bold.SetValue(isolate, value.bold);
        definition.italics.SetValue(isolate, value.italics);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Render::TextBaseSize> Type<Render::TextBaseSize>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Render::TextBaseSize, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto string = definition.string.AtmosValue();
                const auto font = definition.font.AtmosValue();
                const auto wrapWidth = definition.wrapWidth.AtmosValue();
                const auto bold = definition.bold.AtmosValue();
                const auto italics = definition.italics.AtmosValue();
                return string && font && wrapWidth && bold && italics
                    ? Render::TextBaseSize
                    {
                        *string, *font, *wrapWidth, *bold, *italics
                    }
                    : std::optional<Render::TextBaseSize>{};
            });
    }

    Type< Render::TextBaseSize>::Definition::Definition() :
        string("string"),
        font("font"),
        wrapWidth("wrapWidth"),
        bold("bold"),
        italics("italics")
    {}

    AnyObjectDefinition Type<Render::TextBaseSize>::Definition::ToAny()
    {
        return
        {
            {
                { string },
                { font },
                { wrapWidth },
                { bold },
                { italics }
            }
        };
    }
}