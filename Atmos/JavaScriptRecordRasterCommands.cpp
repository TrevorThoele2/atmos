#include "JavaScriptRecordRasterCommands.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Render::Raster::RecordCommands>::ToV8(v8::Isolate& isolate, const Render::Raster::RecordCommands& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.commands.SetValue(isolate, value.commands);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Render::Raster::RecordCommands> Type<Render::Raster::RecordCommands>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Render::Raster::RecordCommands, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto commands = definition.commands.AtmosValue();
                return commands
                    ? Render::Raster::RecordCommands
                    {
                        *commands
                    }
                    : std::optional<Render::Raster::RecordCommands>{};
            });
    }

    Type< Render::Raster::RecordCommands>::Definition::Definition() :
        commands("commands")
    {}

    AnyObjectDefinition Type<Render::Raster::RecordCommands>::Definition::ToAny()
    {
        return
        {
            {
                { commands }
            }
        };
    }
}