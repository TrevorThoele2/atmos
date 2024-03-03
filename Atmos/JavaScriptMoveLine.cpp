#include "JavaScriptMoveLine.h"

#include "JavaScriptNumeric.h"
#include "JavaScriptOptional.h"
#include "JavaScriptVector.h"
#include "JavaScriptPoint2D.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Render::MoveLine>::ToV8(v8::Isolate& isolate, const Render::MoveLine& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.points.SetValue(isolate, value.points);
        definition.z.SetValue(isolate, value.z);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Render::MoveLine> Type<Render::MoveLine>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Render::MoveLine, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                const auto points = definition.points.AtmosValue();
                const auto z = definition.z.AtmosValue();
                return id && points && z
                    ? Render::MoveLine
                    {
                        *id, *points, *z
                    }
                    : std::optional<Render::MoveLine>{};
            });
    }

    Type< Render::MoveLine>::Definition::Definition() :
        id("id"),
        points("points"),
        z("z")
    {}

    AnyObjectDefinition Type<Render::MoveLine>::Definition::ToAny()
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