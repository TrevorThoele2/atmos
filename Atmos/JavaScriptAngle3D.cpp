#include "JavaScriptAngle3D.h"

#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Spatial::Angle3D>::ToV8(v8::Isolate& isolate, const Spatial::Angle3D& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.yaw.SetValue(isolate, value.yaw);
        definition.pitch.SetValue(isolate, value.pitch);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Spatial::Angle3D> Type<Spatial::Angle3D>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Spatial::Angle3D, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto yaw = definition.yaw.AtmosValue();
                const auto pitch = definition.pitch.AtmosValue();
                return yaw && pitch
                    ? Spatial::Angle3D
	                {
	                    *yaw, *pitch
	                }
	                : std::optional<Spatial::Angle3D>{};
            });
    }

    Type<Spatial::Angle3D>::Definition::Definition() :
        yaw("yaw"),
        pitch("pitch")
    {}

    AnyObjectDefinition Type<Spatial::Angle3D>::Definition::ToAny()
    {
        return
        {
            {
                { yaw },
                { pitch }
            }
        };
    }
}