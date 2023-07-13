#include "JavaScriptDrawText.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Render::Raster::DrawText>::ToV8(v8::Isolate& isolate, const Render::Raster::DrawText& value) -> v8::Local<V8T>
    {
        Definition definition;
        const auto font = value.font ? FontAsset{ value.font.ID(), value.font->Name() } : std::optional<FontAsset>();

        definition.shaders.SetValue(isolate, value.shaders);
        definition.string.SetValue(isolate, value.string);
        definition.font.SetValue(isolate, font);
        definition.viewSlice.SetValue(isolate, value.viewSlice);
        definition.color.SetValue(isolate, value.color);
        definition.bold.SetValue(isolate, value.bold);
        definition.italics.SetValue(isolate, value.italics);
        definition.wrapWidth.SetValue(isolate, value.wrapWidth);
        definition.position.SetValue(isolate, value.position);
        definition.rotation.SetValue(isolate, value.rotation);
        definition.scalers.SetValue(isolate, value.scalers);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Render::Raster::DrawText> Type<Render::Raster::DrawText>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto userData = static_cast<UserData*>(isolate.GetData(0));
        const auto reliquary = userData->reliquary;

        return FromObject<Render::Raster::DrawText, Definition>(
            isolate,
            value,
            [&reliquary](const Definition& definition)
            {
                const auto shaders = definition.shaders.AtmosValue();
                const auto string = definition.string.AtmosValue();
                const auto font = definition.font.AtmosValue();
                const auto viewSlice = definition.viewSlice.AtmosValue();
                const auto color = definition.color.AtmosValue();
                const auto bold = definition.bold.AtmosValue();
                const auto italics = definition.italics.AtmosValue();
                const auto wrapWidth = definition.wrapWidth.AtmosValue();
                const auto position = definition.position.AtmosValue();
                const auto rotation = definition.rotation.AtmosValue();
                const auto scalers = definition.scalers.AtmosValue();

                return shaders && string && font && viewSlice && color && bold && italics && wrapWidth && position && rotation && scalers
                    ? Render::Raster::DrawText
                    {
                        nullptr,
                        *shaders,
                        *string,
                        font ? reliquary->Find<Asset::Font>((*font)->id) : Arca::Index<Asset::Font>(),
                        *viewSlice,
                        *color,
                        *bold,
                        *italics,
                        *wrapWidth,
                        *position,
                        *rotation,
                        *scalers
                    }
                    : std::optional<Render::Raster::DrawText>{};
            });
    }

    Type<Render::Raster::DrawText>::Definition::Definition() :
        shaders("shaders"),
        string("string"),
        font("font"),
        viewSlice("viewSlice"),
        color("color"),
        bold("bold"),
        italics("italics"),
        wrapWidth("wrapWidth"),
        position("position"),
        rotation("rotation"),
        scalers("scalers")
    {}

    AnyObjectDefinition Type<Render::Raster::DrawText>::Definition::ToAny()
    {
        return
        {
            {
                { shaders },
                { string },
                { font },
                { viewSlice },
                { color },
                { bold },
                { italics },
                { wrapWidth },
                { position },
                { rotation },
                { scalers }
            }
        };
    }
}