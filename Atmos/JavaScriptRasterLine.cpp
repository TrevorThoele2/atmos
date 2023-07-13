#include "JavaScriptRasterLine.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Render::Raster::Line>::ToV8(v8::Isolate& isolate, const Render::Raster::Line& value) -> v8::Local<V8T>
    {
        Definition definition;
        const auto material = value.material ? MaterialAsset{ value.material.ID(), value.material->Name() } : std::optional<MaterialAsset>();

        definition.material.SetValue(isolate, material);
        definition.points.SetValue(isolate, value.points);
        definition.width.SetValue(isolate, value.width);
        definition.color.SetValue(isolate, value.color);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Render::Raster::Line> Type<Render::Raster::Line>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto userData = static_cast<UserData*>(isolate.GetData(0));
        const auto reliquary = userData->reliquary;

        return FromObject<Render::Raster::Line, Definition>(
            isolate,
            value,
            [&reliquary](const Definition& definition)
            {
                const auto material = definition.material.AtmosValue();
                const auto points = definition.points.AtmosValue();
                const auto width = definition.width.AtmosValue();
                const auto color = definition.color.AtmosValue();
                return material && points && width && color
                    ? Render::Raster::Line
                    {
                        material ? reliquary->Find<Asset::Material>((*material)->id) : Arca::Index<Asset::Material>(),
                        *points,
                        *width,
                        *color,
                    }
                    : std::optional<Render::Raster::Line>{};
            });
    }

    Type<Render::Raster::Line>::Definition::Definition() :
        material("material"),
        points("points"),
        width("width"),
        color("color")
    {}

    AnyObjectDefinition Type<Render::Raster::Line>::Definition::ToAny()
    {
        return
        {
            {
                { material },
                { points },
                { width },
                { color }
            }
        };
    }
}