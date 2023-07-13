#include "JavaScriptExecutingMaterial.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<ExecutingMaterial>::ToV8(v8::Isolate& isolate, const ExecutingMaterial& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.images.SetValue(isolate, value.images);
        definition.texts.SetValue(isolate, value.texts);
        definition.lines.SetValue(isolate, value.lines);
        definition.regions.SetValue(isolate, value.regions);
        definition.recordedCommands.SetValue(isolate, value.recordedCommands);
        return CreateObject(isolate, definition.ToAny());
    }

    Type<ExecutingMaterial>::Definition::Definition() :
        id("id"),
        images("images"),
        texts("texts"),
        lines("lines"),
        regions("regions"),
        recordedCommands("recordedCommands")
    {}

    AnyObjectDefinition Type<ExecutingMaterial>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { images },
                { texts },
                { lines },
                { regions },
                { recordedCommands }
            }
        };
    }

    auto Type<Arca::Index<Render::Raster::ExecutingMaterial>>::ToV8(v8::Isolate& isolate, const Arca::Index<Render::Raster::ExecutingMaterial>& value) -> v8::Local<V8T>
    {
        if (!value)
            return v8::Null(&isolate);

        return JavaScript::ToV8(
            isolate,
            ExecutingMaterial
            {
                value.ID(),
                value->images,
                value->texts,
                value->lines,
                value->regions,
                value->recordedCommands
            });
    }
}