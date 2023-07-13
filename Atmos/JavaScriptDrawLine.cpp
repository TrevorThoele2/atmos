#include "JavaScriptDrawLine.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Render::Raster::DrawLine>::ToV8(v8::Isolate& isolate, const Render::Raster::DrawLine& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.shaders.SetValue(isolate, value.shaders);
        definition.points.SetValue(isolate, value.points);
        definition.width.SetValue(isolate, value.width);
        definition.color.SetValue(isolate, value.color);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Render::Raster::DrawLine> Type<Render::Raster::DrawLine>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto userData = static_cast<UserData*>(isolate.GetData(0));
        const auto reliquary = userData->reliquary;

        return FromObject<Render::Raster::DrawLine, Definition>(
            isolate,
            value,
            [&reliquary](const Definition& definition)
            {
                const auto shaders = definition.shaders.AtmosValue();
                const auto points = definition.points.AtmosValue();
                const auto width = definition.width.AtmosValue();
                const auto color = definition.color.AtmosValue();
                return shaders && points && width && color
                    ? Render::Raster::DrawLine
                    {
                        nullptr,
                        *shaders,
                        *points,
                        *width,
                        *color,
                    }
                    : std::optional<Render::Raster::DrawLine>{};
            });
    }

    Type<Render::Raster::DrawLine>::Definition::Definition() :
        shaders("shaders"),
        points("points"),
        width("width"),
        color("color")
    {}

    AnyObjectDefinition Type<Render::Raster::DrawLine>::Definition::ToAny()
    {
        return
        {
            {
                { shaders },
                { points },
                { width },
                { color }
            }
        };
    }
}