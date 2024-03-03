#include "JavaScriptAxisAlignedBox2D.h"

#include "JavaScriptPoint2D.h"
#include "JavaScriptSize2D.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Spatial::AxisAlignedBox2D>::ToV8(v8::Isolate& isolate, const Spatial::AxisAlignedBox2D& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.center.SetValue(isolate, value.center);
        definition.size.SetValue(isolate, value.size);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Spatial::AxisAlignedBox2D> Type<Spatial::AxisAlignedBox2D>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Spatial::AxisAlignedBox2D, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto center = definition.center.AtmosValue();
                const auto size = definition.size.AtmosValue();
                return center && size
                    ? Spatial::AxisAlignedBox2D
	                {
	                    *center, *size
	                }
	                : std::optional<Spatial::AxisAlignedBox2D>{};
            });
    }

    Type<Spatial::AxisAlignedBox2D>::Definition::Definition() :
        center("center"),
        size("size")
    {}

    AnyObjectDefinition Type<Spatial::AxisAlignedBox2D>::Definition::ToAny()
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