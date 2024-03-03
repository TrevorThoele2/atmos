#include "JavaScriptToWorldPoint3D.h"

#include "JavaScriptPoint2D.h"
#include "JavaScriptNumeric.h"
#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Spatial::ToWorldPoint3D>::ToV8(v8::Isolate& isolate, const Spatial::ToWorldPoint3D& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.from.SetValue(isolate, value.from);
        definition.z.SetValue(isolate, value.z);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Spatial::ToWorldPoint3D> Type<Spatial::ToWorldPoint3D>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Spatial::ToWorldPoint3D, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto from = definition.from.AtmosValue();
                const auto z = definition.z.AtmosValue();
                return from && z
                    ? Spatial::ToWorldPoint3D
                    {
                        *from, *z
                    }
                    : std::optional<Spatial::ToWorldPoint3D>{};
            });
    }

    Type<Spatial::ToWorldPoint3D>::Definition::Definition() :
        from("from"),
        z("z")
    {}

    AnyObjectDefinition Type<Spatial::ToWorldPoint3D>::Definition::ToAny()
    {
        return
        {
            {
                { from },
                { z }
            }
        };
    }
}