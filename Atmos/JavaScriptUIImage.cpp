#include "JavaScriptUIImage.h"

#include "JavaScriptNumeric.h"
#include "JavaScriptOptional.h"
#include "JavaScriptPoint3D.h"
#include "JavaScriptColor.h"
#include "JavaScriptScalers2D.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<UIImage>::ToV8(v8::Isolate& isolate, const UIImage& value) -> v8::Local<V8T>
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

    std::optional<UIImage> Type<UIImage>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<UIImage, Definition>(
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
                    ? UIImage
                    {
                        *id, *asset, *assetIndex, *material, *color, *position, *scalers, *rotation
                    }
                    : std::optional<UIImage>{};
            });
    }

    Type<UIImage>::Definition::Definition() :
        id("id"),
        asset("asset"),
        assetIndex("assetIndex"),
        material("material"),
        color("color"),
        position("position"),
        scalers("scalers"),
        rotation("rotation")
    {}

    AnyObjectDefinition Type<UIImage>::Definition::ToAny()
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

    auto Type<Arca::Index<UI::Image>>::ToV8(v8::Isolate& isolate, const Arca::Index<UI::Image>& value) -> v8::Local<V8T>
    {
        if (!value)
            return v8::Null(&isolate);

        const auto asset = value->Asset();
        const auto material = value->Material();
        return JavaScript::ToV8(
            isolate,
            UIImage
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

    std::optional<CreateUIImage> Type<CreateUIImage>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<CreateUIImage, Definition>(
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
                    ? CreateUIImage
                    {
                        *asset, *assetIndex, *material, *color, *position, *scalers, *rotation
                    }
                    : std::optional<CreateUIImage>{};
            });
    }

    Type<CreateUIImage>::Definition::Definition() :
        asset("asset"),
        assetIndex("assetIndex"),
        material("material"),
        color("color"),
        position("position"),
        scalers("scalers"),
        rotation("rotation")
    {}

    AnyObjectDefinition Type<CreateUIImage>::Definition::ToAny()
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

    std::optional<Arca::Create<UI::Image>> Type<Arca::Create<UI::Image>>::FromV8(
        v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto userData = static_cast<UserData*>(isolate.GetData(0));
        const auto reliquary = userData->reliquary;

        const auto command = JavaScript::FromV8<CreateUIImage>(isolate, value);
        return command
            ? Arca::Create<UI::Image>(
                command->asset ? reliquary->Find<Asset::Image>(command->asset->id) : Arca::Index<Asset::Image>(),
                command->assetIndex,
                command->material ? reliquary->Find<Asset::Material>(command->material->id) : Arca::Index<Asset::Material>(),
                command->color,
                command->position,
                command->scalers,
                command->rotation)
            : std::optional<Arca::Create<UI::Image>>{};
    }
}