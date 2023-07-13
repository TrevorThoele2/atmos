#include "JavaScriptRasterImage.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Render::Raster::Image>::ToV8(v8::Isolate& isolate, const Render::Raster::Image& value) -> v8::Local<V8T>
    {
        Definition definition;
        const auto material = value.material ? MaterialAsset { value.material.ID(), value.material->Name() } : std::optional<MaterialAsset>();
        const auto asset = value.asset ? ImageAsset{ value.asset.ID(), value.asset->Name() } : std::optional<ImageAsset>();

        definition.material.SetValue(isolate, material);
        definition.asset.SetValue(isolate, asset);
        definition.assetSlice.SetValue(isolate, value.assetSlice);
        definition.viewSlice.SetValue(isolate, value.viewSlice);
        definition.color.SetValue(isolate, value.color);
        definition.position.SetValue(isolate, value.position);
        definition.size.SetValue(isolate, value.size);
        definition.rotation.SetValue(isolate, value.rotation);
        definition.scalers.SetValue(isolate, value.scalers);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Render::Raster::Image> Type<Render::Raster::Image>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto userData = static_cast<UserData*>(isolate.GetData(0));
        const auto reliquary = userData->reliquary;
        
        return FromObject<Render::Raster::Image, Definition>(
            isolate,
            value,
            [&reliquary](const Definition& definition)
            {
                const auto material = definition.material.AtmosValue();
                const auto asset = definition.asset.AtmosValue();
                const auto assetSlice = definition.assetSlice.AtmosValue();
                const auto viewSlice = definition.viewSlice.AtmosValue();
                const auto color = definition.color.AtmosValue();
                const auto position = definition.position.AtmosValue();
                const auto size = definition.size.AtmosValue();
                const auto rotation = definition.rotation.AtmosValue();
                const auto scalers = definition.scalers.AtmosValue();
                return material && asset && assetSlice && viewSlice && color && position && size && rotation && scalers
                    ? Render::Raster::Image
                    {
                        material ? reliquary->Find<Asset::Material>((*material)->id) : Arca::Index<Asset::Material>(),
                        asset ? reliquary->Find<Asset::Image>((*asset)->id) : Arca::Index<Asset::Image>(),
                        *assetSlice,
                        *viewSlice,
                        *color,
                        *position,
                        *size,
                        *rotation,
                        *scalers
                    }
                    : std::optional<Render::Raster::Image>{};
            });
    }
    
    Type<Render::Raster::Image>::Definition::Definition() :
        material("material"),
        asset("asset"),
        assetSlice("assetSlice"),
        viewSlice("viewSlice"),
        color("color"),
        position("position"),
        size("size"),
        rotation("rotation"),
        scalers("scalers")
    {}

    AnyObjectDefinition Type<Render::Raster::Image>::Definition::ToAny()
    {
        return
        {
            {
                { material },
                { asset },
                { assetSlice },
                { viewSlice },
                { color },
                { position },
                { size },
                { rotation },
                { scalers }
            }
        };
    }
}