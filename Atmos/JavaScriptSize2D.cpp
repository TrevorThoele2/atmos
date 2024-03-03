#include "JavaScriptSize2D.h"

#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Spatial::Size2D>::ToV8(v8::Isolate& isolate, const Spatial::Size2D& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.width.SetValue(isolate, value.width);
        definition.height.SetValue(isolate, value.height);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Spatial::Size2D> Type<Spatial::Size2D>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Spatial::Size2D, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto width = definition.width.AtmosValue();
                const auto height = definition.height.AtmosValue();
                return width && height
                    ? Spatial::Size2D
	                {
	                    *width, *height
	                }
	                : std::optional<Spatial::Size2D>{};
            });
    }

    Type<Spatial::Size2D>::Definition::Definition() :
        width("width"),
        height("height")
    {}

    AnyObjectDefinition Type<Spatial::Size2D>::Definition::ToAny()
    {
        return
        {
            {
                { width },
                { height }
            }
        };
    }
}