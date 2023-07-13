#include "JavaScriptDynamicImage.h"

#include "JavaScriptNumeric.h"
#include "JavaScriptOptional.h"
#include "JavaScriptPoint3D.h"
#include "JavaScriptColor.h"
#include "JavaScriptScalers2D.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<DynamicImage>::ToV8(v8::Isolate& isolate, const DynamicImage& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.asset.SetValue(isolate, value.asset);
        definition.assetIndex.SetValue(isolate, value.assetIndex);
        definition.material.SetValue(isolate, value.material);
        definition.color.SetValue(isolate, value.color);
        definition.position.SetValue(isolate, value.position);
        definition.scalers.SetValue(isolate, value.scalers);
        definition.rotation.SetValue(isolate, value.rotation);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<DynamicImage> Type<DynamicImage>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<DynamicImage, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                const auto asset = definition.asset.AtmosValue();
                const auto assetIndex = definition.assetIndex.AtmosValue();
                const auto material = definition.material.AtmosValue();
                const auto color = definition.color.AtmosValue();
                const auto position = definition.position.AtmosValue();
                const auto scalers = definition.scalers.AtmosValue();
                const auto rotation = definition.rotation.AtmosValue();
                return id && asset && assetIndex && material && color && position && scalers && rotation
                    ? DynamicImage
                    {
                        *id, *asset, *assetIndex, *material, *color, *position, *scalers, *rotation
                    }
                    : std::optional<DynamicImage>{};
            });
    }

    Type<DynamicImage>::Definition::Definition() :
        id("id"),
        asset("asset"),
        assetIndex("assetIndex"),
        material("material"),
        color("color"),
        position("position"),
        scalers("scalers"),
        rotation("rotation")
    {}

    AnyObjectDefinition Type<DynamicImage>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { asset },
                { assetIndex },
                { material },
                { color },
                { position },
                { scalers },
                { rotation }
            }
        };
    }

    auto Type<Arca::Index<Render::DynamicImage>>::ToV8(v8::Isolate& isolate, const Arca::Index<Render::DynamicImage>& value) -> v8::Local<V8T>
    {
        if (!value)
            return v8::Null(&isolate);

        const auto asset = value->Asset();
        const auto material = value->Material();
        return JavaScript::ToV8(
            isolate,
            DynamicImage
            {
                value.ID(),
                asset
                    ? ImageAsset
                    {
                        asset.ID(),
                        asset->Name()
                    }
                    : std::optional<ImageAsset>(),
                value->AssetIndex(),
                material
                    ? MaterialAsset
                    {
                        material.ID(),
                        material->Name()
                    }
                    : std::optional<MaterialAsset>(),
                value->Color(),
                value->Position(),
                value->Scalers(),
                value->Rotation()
            });
    }

    std::optional<CreateDynamicImage> Type<CreateDynamicImage>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<CreateDynamicImage, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto asset = definition.asset.AtmosValue();
                const auto assetIndex = definition.assetIndex.AtmosValue();
                const auto material = definition.material.AtmosValue();
                const auto color = definition.color.AtmosValue();
                const auto position = definition.position.AtmosValue();
                const auto scalers = definition.scalers.AtmosValue();
                const auto rotation = definition.rotation.AtmosValue();
                return asset && assetIndex && material && color && position && scalers && rotation
                    ? CreateDynamicImage
                {
                    *asset, *assetIndex, *material, *color, *position, *scalers, *rotation
                }
                : std::optional<CreateDynamicImage>{};
            });
    }

    Type<CreateDynamicImage>::Definition::Definition() :
        asset("asset"),
        assetIndex("assetIndex"),
        material("material"),
        color("color"),
        position("position"),
        scalers("scalers"),
        rotation("rotation")
    {}

    AnyObjectDefinition Type<CreateDynamicImage>::Definition::ToAny()
    {
        return
        {
            {
                { asset },
                { assetIndex },
                { material },
                { color },
                { position },
                { scalers },
                { rotation }
            }
        };
    }

    std::optional<Arca::Create<Render::DynamicImage>> Type<Arca::Create<Render::DynamicImage>>::FromV8(
        v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto userData = static_cast<UserData*>(isolate.GetData(0));
        const auto reliquary = userData->reliquary;

        const auto command = JavaScript::FromV8<CreateDynamicImage>(isolate, value);
        return command
            ? Arca::Create<Render::DynamicImage>(
                command->asset ? reliquary->Find<Asset::Image>(command->asset->id) : Arca::Index<Asset::Image>(),
                command->assetIndex,
                command->material ? reliquary->Find<Asset::Material>(command->material->id) : Arca::Index<Asset::Material>(),
                command->color,
                command->position,
                command->scalers,
                command->rotation)
            : std::optional<Arca::Create<Render::DynamicImage>>{};
    }
}