#include "JavaScriptLine.h"

#include "JavaScriptNumeric.h"
#include "JavaScriptPoint2D.h"
#include "JavaScriptOptional.h"
#include "JavaScriptVector.h"
#include "JavaScriptColor.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Line>::ToV8(v8::Isolate& isolate, const Line& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.points.SetValue(isolate, value.points);
        definition.z.SetValue(isolate, value.z);
        definition.material.SetValue(isolate, value.material);
        definition.width.SetValue(isolate, value.width);
        definition.color.SetValue(isolate, value.color);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Line> Type<Line>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Line, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                const auto points = definition.points.AtmosValue();
                const auto z = definition.z.AtmosValue();
                const auto material = definition.material.AtmosValue();
                const auto width = definition.width.AtmosValue();
                const auto color = definition.color.AtmosValue();
                return id && points && z && material
                    ? Line
                    {
                        *id, *points, *z, *material, *width, *color
                    }
                    : std::optional<Line>{};
            });
    }

    Type<Line>::Definition::Definition() :
        id("id"),
        points("points"),
        z("z"),
        material("material"),
        width("width"),
        color("color")
    {}

    AnyObjectDefinition Type<Line>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { points },
                { z },
                { material },
                { width },
                { color }
            }
        };
    }

    auto Type<Arca::Index<Render::Line>>::ToV8(v8::Isolate& isolate, const Arca::Index<Render::Line>& value) -> v8::Local<V8T>
    {
        if (!value)
            return v8::Null(&isolate);

        const auto material = value->renderCore->material;
        return JavaScript::ToV8(
            isolate,
            Line
            {
                value.ID(),
                value->points,
                value->z,
                material
                    ? MaterialAsset
                    {
                        material.ID(),
                        material->Name()
                    }
                    : std::optional<MaterialAsset>(),
                value->width,
                value->renderCore->color
            });
    }

    std::optional<CreateLine> Type<CreateLine>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<CreateLine, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto points = definition.points.AtmosValue();
                const auto z = definition.z.AtmosValue();
                const auto material = definition.material.AtmosValue();
                const auto width = definition.width.AtmosValue();
                const auto color = definition.color.AtmosValue();
                return points && z && material
                    ? CreateLine
                    {
                        *points, *z, *material, *width, *color
                    }
                    : std::optional<CreateLine>{};
            });
    }

    Type<CreateLine>::Definition::Definition() :
        points("points"),
        z("z"),
        material("material"),
        width("width"),
        color("color")
    {}

    AnyObjectDefinition Type<CreateLine>::Definition::ToAny()
    {
        return
        {
            {
                { points },
                { z },
                { material },
                { width },
                { color }
            }
        };
    }

    std::optional<Arca::Create<Render::Line>> Type<Arca::Create<Render::Line>>::FromV8(
        v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto userData = static_cast<UserData*>(isolate.GetData(0));
        const auto reliquary = userData->reliquary;

        const auto command = JavaScript::FromV8<CreateLine>(isolate, value);
        return command
            ? Arca::Create<Render::Line>(
                command->points,
                command->z,
                command->material ? reliquary->Find<Asset::Material>(command->material->id) : Arca::Index<Asset::Material>(),
                command->width,
                command->color)
            : std::optional<Arca::Create<Render::Line>>{};
    }
}