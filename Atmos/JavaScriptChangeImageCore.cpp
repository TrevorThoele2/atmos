#include "JavaScriptChangeImageCore.h"

#include "JavaScriptNumeric.h"
#include "JavaScriptOptional.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Render::ChangeImageCore>::ToV8(v8::Isolate& isolate, const Render::ChangeImageCore& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.asset.SetValue(isolate, value.asset ? ImageAsset(value.asset->ID(), (*value.asset)->Name()) : std::optional<ImageAsset>{});
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Render::ChangeImageCore> Type<Render::ChangeImageCore>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Render::ChangeImageCore, Definition>(
            isolate,
            value,
            [&isolate](const Definition& definition)
            {
                const auto userData = static_cast<UserData*>(isolate.GetData(0));
                const auto reliquary = userData->reliquary;

                const auto id = definition.id.AtmosValue();
                const auto asset = definition.asset.AtmosValue();
                const auto assetIndex = definition.assetIndex.AtmosValue();
                return id && asset && assetIndex
                    ? Render::ChangeImageCore
                    {
                        *id, *asset ? reliquary->Find<Asset::Image>((*asset)->id) : Arca::Index<Asset::Image>(), *assetIndex
                    }
                    : std::optional<Render::ChangeImageCore>{};
            });
    }

    Type< Render::ChangeImageCore>::Definition::Definition() :
        id("id"),
        asset("asset"),
        assetIndex("assetIndex")
    {}

    AnyObjectDefinition Type<Render::ChangeImageCore>::Definition::ToAny()
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
}