#include "JavaScriptMoveGridRegion.h"

#include "JavaScriptUnorderedSet.h"
#include "JavaScriptNumeric.h"
#include "JavaScriptOptional.h"
#include "JavaScriptGridPoint.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Render::MoveGridRegion>::ToV8(v8::Isolate& isolate, const Render::MoveGridRegion& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.points.SetValue(isolate, value.points);
        definition.z.SetValue(isolate, value.z);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Render::MoveGridRegion> Type<Render::MoveGridRegion>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Render::MoveGridRegion, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                const auto points = definition.points.AtmosValue();
                const auto z = definition.z.AtmosValue();
                return id && points && z
                    ? Render::MoveGridRegion
                    {
                        *id, *points, *z
                    }
                    : std::optional<Render::MoveGridRegion>{};
            });
    }

    Type< Render::MoveGridRegion>::Definition::Definition() :
        id("id"),
        points("points"),
        z("z")
    {}

    AnyObjectDefinition Type<Render::MoveGridRegion>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { points },
                { z }
            }
        };
    }
}