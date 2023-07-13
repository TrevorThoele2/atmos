#include "JavaScriptRasterRegion.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Render::Raster::Region>::ToV8(v8::Isolate& isolate, const Render::Raster::Region& value) -> v8::Local<V8T>
    {
        Definition definition;
        const auto material = value.material ? MaterialAsset{ value.material.ID(), value.material->Name() } : std::optional<MaterialAsset>();

        definition.material.SetValue(isolate, material);
        definition.mesh.SetValue(isolate, value.mesh);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Render::Raster::Region> Type<Render::Raster::Region>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto userData = static_cast<UserData*>(isolate.GetData(0));
        const auto reliquary = userData->reliquary;

        return FromObject<Render::Raster::Region, Definition>(
            isolate,
            value,
            [&reliquary](const Definition& definition)
            {
                const auto material = definition.material.AtmosValue();
                const auto mesh = definition.mesh.AtmosValue();
                return material && mesh
                    ? Render::Raster::Region
                    {
                        material ? reliquary->Find<Asset::Material>((*material)->id) : Arca::Index<Asset::Material>(),
                        *mesh,
                    }
                    : std::optional<Render::Raster::Region>{};
            });
    }

    Type<Render::Raster::Region>::Definition::Definition() :
        material("material"),
        mesh("mesh")
    {}

    AnyObjectDefinition Type<Render::Raster::Region>::Definition::ToAny()
    {
        return
        {
            {
                { material },
                { mesh }
            }
        };
    }
}