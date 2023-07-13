#include "JavaScriptRasterCommand.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Render::Raster::Command>::ToV8(v8::Isolate& isolate, const Render::Raster::Command& value) -> v8::Local<V8T>
    {
        return std::visit(
            [&isolate](auto value) -> v8::Local<v8::Value>
            {
                return JavaScript::ToV8(isolate, value);
            },
            value);
    }

    std::optional<Render::Raster::Command> Type<Render::Raster::Command>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        auto drawImage = JavaScript::FromV8<Render::Raster::DrawImage>(isolate, value);
        if (drawImage)
            return *drawImage;

        auto drawLine = JavaScript::FromV8<Render::Raster::DrawLine>(isolate, value);
        if (drawLine)
            return *drawLine;

        auto drawText = JavaScript::FromV8<Render::Raster::DrawText>(isolate, value);
        if (drawText)
            return *drawText;

        auto drawRegion = JavaScript::FromV8<Render::Raster::DrawRegion>(isolate, value);
        if (drawRegion)
            return *drawRegion;

        return {};
    }
}