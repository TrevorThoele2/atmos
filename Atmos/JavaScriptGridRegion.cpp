#include "JavaScriptGridRegion.h"

#include "JavaScriptNumeric.h"
#include "JavaScriptGridPoint.h"
#include "JavaScriptOptional.h"
#include "JavaScriptUnorderedSet.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<GridRegion>::ToV8(v8::Isolate& isolate, const GridRegion& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.points.SetValue(isolate, value.points);
        definition.z.SetValue(isolate, value.z);
        definition.material.SetValue(isolate, value.material);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<GridRegion> Type<GridRegion>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<GridRegion, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                const auto points = definition.points.AtmosValue();
                const auto z = definition.z.AtmosValue();
                const auto material = definition.material.AtmosValue();
                return id && points && z && material
                    ? GridRegion
                    {
                        *id, *points, *z, *material
                    }
                    : std::optional<GridRegion>{};
            });
    }

    Type<GridRegion>::Definition::Definition() :
        id("id"),
        points("points"),
        z("z"),
        material("material")
    {}

    AnyObjectDefinition Type<GridRegion>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { points },
                { z },
                { material }
            }
        };
    }

    auto Type<Arca::Index<Render::GridRegion>>::ToV8(v8::Isolate& isolate, const Arca::Index<Render::GridRegion>& value) -> v8::Local<V8T>
    {
        if (!value)
            return v8::Null(&isolate);

        const auto material = value->renderCore->material;
        return JavaScript::ToV8(
            isolate,
            GridRegion
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
                    : std::optional<MaterialAsset>()
            });
    }

    std::optional<CreateGridRegion> Type<CreateGridRegion>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<CreateGridRegion, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto points = definition.points.AtmosValue();
                const auto z = definition.z.AtmosValue();
                const auto material = definition.material.AtmosValue();
                return points && z && material
                    ? CreateGridRegion
                    {
                        *points, *z, *material
                    }
                    : std::optional<CreateGridRegion>{};
            });
    }

    Type<CreateGridRegion>::Definition::Definition() :
        points("points"),
        z("z"),
        material("material")
    {}

    AnyObjectDefinition Type<CreateGridRegion>::Definition::ToAny()
    {
        return
        {
            {
                { points },
                { z },
                { material }
            }
        };
    }

    std::optional<Arca::Create<Render::GridRegion>> Type<Arca::Create<Render::GridRegion>>::FromV8(
        v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto userData = static_cast<UserData*>(isolate.GetData(0));
        const auto reliquary = userData->reliquary;

        const auto command = JavaScript::FromV8<CreateGridRegion>(isolate, value);
        return command
            ? Arca::Create<Render::GridRegion>(
                std::unordered_set<Spatial::Grid::Point>(command->points.begin(), command->points.end()),
                command->z,
                command->material ? reliquary->Find<Asset::Material>(command->material->id) : Arca::Index<Asset::Material>())
            : std::optional<Arca::Create<Render::GridRegion>>{};
    }
}