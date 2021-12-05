#include "JavaScriptGridAxisAlignedBox.h"

#include "JavaScriptGridPoint.h"
#include "JavaScriptGridSize.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Spatial::Grid::AxisAlignedBox>::ToV8(v8::Isolate& isolate, const Spatial::Grid::AxisAlignedBox& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.center.SetValue(isolate, value.center);
        definition.size.SetValue(isolate, value.size);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Spatial::Grid::AxisAlignedBox> Type<Spatial::Grid::AxisAlignedBox>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Spatial::Grid::AxisAlignedBox, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto center = definition.center.AtmosValue();
                const auto size = definition.size.AtmosValue();
                return center && size
                    ? Spatial::Grid::AxisAlignedBox
                {
                    *center, *size
                }
                : std::optional<Spatial::Grid::AxisAlignedBox>{};
            });
    }

    Type<Spatial::Grid::AxisAlignedBox>::Definition::Definition() :
        center("center"),
        size("size")
    {}

    AnyObjectDefinition Type<Spatial::Grid::AxisAlignedBox>::Definition::ToAny()
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