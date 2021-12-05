#include "JavaScriptSize3D.h"

#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Spatial::Size3D>::ToV8(v8::Isolate& isolate, const Spatial::Size3D& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.width.SetValue(isolate, value.width);
        definition.height.SetValue(isolate, value.height);
        definition.depth.SetValue(isolate, value.depth);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Spatial::Size3D> Type<Spatial::Size3D>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Spatial::Size3D, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto width = definition.width.AtmosValue();
                const auto height = definition.height.AtmosValue();
                const auto depth = definition.depth.AtmosValue();
                return width && height && depth
                    ? Spatial::Size3D
	                {
	                    *width, *height, *depth
	                }
	                : std::optional<Spatial::Size3D>{};
            });
    }

    Type<Spatial::Size3D>::Definition::Definition() :
        width("width"),
        height("height"),
		depth("depth")
    {}

    AnyObjectDefinition Type<Spatial::Size3D>::Definition::ToAny()
    {
        return
        {
            {
                { width },
                { height },
                { depth }
            }
        };
    }
}