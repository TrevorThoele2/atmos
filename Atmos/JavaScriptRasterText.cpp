#include "JavaScriptRasterText.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Render::Raster::Text>::ToV8(v8::Isolate& isolate, const Render::Raster::Text& value) -> v8::Local<V8T>
    {
        Definition definition;
        const auto material = value.material ? MaterialAsset{ value.material.ID(), value.material->Name() } : std::optional<MaterialAsset>();
        const auto font = value.font ? FontAsset{ value.font.ID(), value.font->Name() } : std::optional<FontAsset>();
        
        definition.material.SetValue(isolate, material);
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

    std::optional<Render::Raster::Text> Type<Render::Raster::Text>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto userData = static_cast<UserData*>(isolate.GetData(0));
        const auto reliquary = userData->reliquary;

        return FromObject<Render::Raster::Text, Definition>(
            isolate,
            value,
            [&reliquary](const Definition& definition)
            {
                const auto material = definition.material.AtmosValue();
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

                return material && string && font && viewSlice && color && bold && italics && wrapWidth && position && rotation && scalers
                    ? Render::Raster::Text
                    {
                        material ? reliquary->Find<Asset::Material>((*material)->id) : Arca::Index<Asset::Material>(),
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
                    : std::optional<Render::Raster::Text>{};
            });
    }
    
    Type<Render::Raster::Text>::Definition::Definition() :
        material("material"),
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

    AnyObjectDefinition Type<Render::Raster::Text>::Definition::ToAny()
    {
        return
        {
            {
                { material },
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