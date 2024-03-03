#include "JavaScriptPoint3D.h"

#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Spatial::Point3D>::ToV8(v8::Isolate& isolate, const Spatial::Point3D& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.x.SetValue(isolate, value.x);
        definition.y.SetValue(isolate, value.y);
        definition.z.SetValue(isolate, value.z);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Spatial::Point3D> Type<Spatial::Point3D>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Spatial::Point3D, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto x = definition.x.AtmosValue();
                const auto y = definition.y.AtmosValue();
                const auto z = definition.z.AtmosValue();
            	return x && y && z
                    ? Spatial::Point3D
	                {
	                    *x, *y, *z
	                }
	                : std::optional<Spatial::Point3D>{};
            });
    }

    Type<Spatial::Point3D>::Definition::Definition() :
        x("x"),
        y("y"),
		z("z")
    {}

    AnyObjectDefinition Type<Spatial::Point3D>::Definition::ToAny()
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