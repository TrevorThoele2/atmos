#include "JavaScriptScalers3D.h"

#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Spatial::Scalers3D>::ToV8(v8::Isolate& isolate, const Spatial::Scalers3D& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.x.SetValue(isolate, value.x);
        definition.y.SetValue(isolate, value.y);
        definition.z.SetValue(isolate, value.z);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Spatial::Scalers3D> Type<Spatial::Scalers3D>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Spatial::Scalers3D, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto x = definition.x.AtmosValue();
                const auto y = definition.y.AtmosValue();
                const auto z = definition.z.AtmosValue();
                return x && y
                    ? Spatial::Scalers3D
	                {
	                    *x, *y, *z
	                }
	                : std::optional<Spatial::Scalers3D>{};
            });
    }

    Type<Spatial::Scalers3D>::Definition::Definition() :
        x("x"),
        y("y"),
		z("z")
    {}

    AnyObjectDefinition Type<Spatial::Scalers3D>::Definition::ToAny()
    {
        return
        {
            {
                { x },
                { y },
                { z }
            }
        };
    }
}