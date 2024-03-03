#include "JavaScriptAxisAlignedBox3D.h"

#include "JavaScriptPoint3D.h"
#include "JavaScriptSize3D.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Spatial::AxisAlignedBox3D>::ToV8(v8::Isolate& isolate, const Spatial::AxisAlignedBox3D& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.center.SetValue(isolate, value.center);
        definition.size.SetValue(isolate, value.size);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Spatial::AxisAlignedBox3D> Type<Spatial::AxisAlignedBox3D>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Spatial::AxisAlignedBox3D, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto center = definition.center.AtmosValue();
                const auto size = definition.size.AtmosValue();
                return center && size
                    ? Spatial::AxisAlignedBox3D
	                {
	                    *center, *size
	                }
	                : std::optional<Spatial::AxisAlignedBox3D>{};
            });
    }

    Type<Spatial::AxisAlignedBox3D>::Definition::Definition() :
        center("center"),
        size("size")
    {}

    AnyObjectDefinition Type<Spatial::AxisAlignedBox3D>::Definition::ToAny()
    {
        return
        {
            {
                { center },
                { size }
            }
        };
    }
}