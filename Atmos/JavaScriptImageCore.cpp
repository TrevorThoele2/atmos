#include "JavaScriptImageCore.h"

#include "JavaScriptNumeric.h"
#include "JavaScriptOptional.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<ImageCore>::ToV8(v8::Isolate& isolate, const ImageCore& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.asset.SetValue(isolate, value.asset);
        definition.assetIndex.SetValue(isolate, value.assetIndex);
        return CreateObject(isolate, definition.ToAny());
    }

    Type<ImageCore>::Definition::Definition() :
        id("id"),
        asset("asset"),
        assetIndex("assetIndex")
    {}

    AnyObjectDefinition Type<ImageCore>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { asset },
                { assetIndex }
            }
        };
    }

    auto Type<Arca::Index<Render::ImageCore>>::ToV8(v8::Isolate& isolate, const Arca::Index<Render::ImageCore>& value) -> v8::Local<V8T>
    {
        if (!value)
            return v8::Null(&isolate);

        const auto asset = value->asset;
        return JavaScript::ToV8(
            isolate,
            ImageCore
            {
                value.ID(),
                asset
                    ? ImageAsset
                    {
                        asset.ID(),
                        asset->Name()
                    }
                    : std::optional<ImageAsset>(),
                value->assetIndex
            });
    }

    std::optional<CreateImageCore> Type<CreateImageCore>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<CreateImageCore, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                const auto asset = definition.asset.AtmosValue();
                const auto assetIndex = definition.assetIndex.AtmosValue();
                return id && asset && assetIndex
                    ? CreateImageCore
                    {
                        *id, *asset, *assetIndex
                    }
                    : std::optional<CreateImageCore>{};
            });
    }

    Type<CreateImageCore>::Definition::Definition() :
        id("id"),
        asset("asset"),
        assetIndex("assetIndex")
    {}

    AnyObjectDefinition Type<CreateImageCore>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { asset },
                { assetIndex }
            }
        };
    }

    std::optional<Arca::Create<Render::ImageCore>> Type<Arca::Create<Render::ImageCore>>::FromV8(
        v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto userData = static_cast<UserData*>(isolate.GetData(0));
        const auto reliquary = userData->reliquary;

        const auto command = JavaScript::FromV8<CreateImageCore>(isolate, value);
        return command
            ? Arca::Create<Render::ImageCore>(
                command->id,
                command->asset ? reliquary->Find<Asset::Image>(command->asset->id) : Arca::Index<Asset::Image>(),
                command->assetIndex)
            : std::optional<Arca::Create<Render::ImageCore>>{};
    }
}