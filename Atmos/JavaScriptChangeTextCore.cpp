#include "JavaScriptChangeTextCore.h"

#include "JavaScriptBool.h"
#include "JavaScriptNumeric.h"
#include "JavaScriptString.h"
#include "JavaScriptOptional.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Render::ChangeTextCore>::ToV8(v8::Isolate& isolate, const Render::ChangeTextCore& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.string.SetValue(isolate, value.string);
        definition.font.SetValue(
            isolate, value.font ? FontAsset(value.font->ID(), (*value.font)->Name()) : std::optional<FontAsset>{});
        definition.wrapWidth.SetValue(isolate, value.wrapWidth);
        definition.bold.SetValue(isolate, value.bold);
        definition.italics.SetValue(isolate, value.italics);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Render::ChangeTextCore> Type<Render::ChangeTextCore>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Render::ChangeTextCore, Definition>(
            isolate,
            value,
            [&isolate](const Definition& definition)
            {
                const auto userData = static_cast<UserData*>(isolate.GetData(0));
                const auto reliquary = userData->reliquary;

                const auto id = definition.id.AtmosValue();
                const auto string = definition.string.AtmosValue();
                const auto font = definition.font.AtmosValue();
                const auto wrapWidth = definition.wrapWidth.AtmosValue();
                const auto bold = definition.bold.AtmosValue();
                const auto italics = definition.italics.AtmosValue();
                return id && string && font && wrapWidth && bold && italics
                    ? Render::ChangeTextCore
                    {
                        *id, *string, *font ? reliquary->Find<Asset::Font>((*font)->id) : Arca::Index<Asset::Font>(), *wrapWidth, *bold, *italics
                    }
                    : std::optional<Render::ChangeTextCore>{};
            });
    }

    Type< Render::ChangeTextCore>::Definition::Definition() :
        id("id"),
        string("string"),
        font("font"),
        wrapWidth("wrapWidth"),
        bold("bold"),
        italics("italics")
    {}

    AnyObjectDefinition Type<Render::ChangeTextCore>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { string },
                { font },
                { wrapWidth },
                { bold },
                { italics }
            }
        };
    }
}