#pragma once

#include "JavaScriptType.h"
#include "RecordRasterCommands.h"

#include "JavaScriptRasterCommand.h"
#include "JavaScriptVariant.h"
#include "JavaScriptVector.h"
#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Render::Raster::RecordCommands>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Render::Raster::RecordCommands& value);
        [[nodiscard]] static std::optional<Render::Raster::RecordCommands> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<std::vector<Render::Raster::Command>> commands;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}