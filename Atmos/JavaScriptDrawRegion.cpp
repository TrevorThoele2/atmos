#include "JavaScriptDrawRegion.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Render::Raster::DrawRegion>::ToV8(v8::Isolate& isolate, const Render::Raster::DrawRegion& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.shaders.SetValue(isolate, value.shaders);
        definition.mesh.SetValue(isolate, value.mesh);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Render::Raster::DrawRegion> Type<Render::Raster::DrawRegion>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto userData = static_cast<UserData*>(isolate.GetData(0));
        const auto reliquary = userData->reliquary;

        return FromObject<Render::Raster::DrawRegion, Definition>(
            isolate,
            value,
            [&reliquary](const Definition& definition)
            {
                const auto shaders = definition.shaders.AtmosValue();
                const auto mesh = definition.mesh.AtmosValue();
                return shaders && mesh
                    ? Render::Raster::DrawRegion
                    {
                        nullptr,
                        *shaders,
                        *mesh,
                    }
                    : std::optional<Render::Raster::DrawRegion>{};
            });
    }

    Type<Render::Raster::DrawRegion>::Definition::Definition() :
        shaders("shaders"),
        mesh("mesh")
    {}

    AnyObjectDefinition Type<Render::Raster::DrawRegion>::Definition::ToAny()
    {
        return
        {
            {
                { shaders },
                { mesh }
            }
        };
    }
}