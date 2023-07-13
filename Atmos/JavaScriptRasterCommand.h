#pragma once

#include "JavaScriptType.h"
#include "RasterCommand.h"

#include "JavaScriptDrawImage.h"
#include "JavaScriptDrawLine.h"
#include "JavaScriptDrawText.h"
#include "JavaScriptDrawRegion.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Render::Raster::Command>
    {
    public:
        using V8T = v8::Value;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Render::Raster::Command& value);
        [[nodiscard]] static std::optional<Render::Raster::Command> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };
}