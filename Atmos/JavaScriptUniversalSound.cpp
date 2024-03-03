#include "JavaScriptUniversalSound.h"

#include "JavaScriptNumeric.h"
#include "JavaScriptAudioAsset.h"
#include "JavaScriptOptional.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<UniversalSound>::ToV8(v8::Isolate& isolate, const UniversalSound& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.asset.SetValue(isolate, value.asset);
        definition.volume.SetValue(isolate, value.volume);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<UniversalSound> Type<UniversalSound>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<UniversalSound, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                const auto asset = definition.asset.AtmosValue();
                const auto volume = definition.volume.AtmosValue();
                return id && asset && volume
                    ? UniversalSound
                    {
                        *id, *asset, *volume
                    }
                    : std::optional<UniversalSound>{};
            });
    }

    Type<UniversalSound>::Definition::Definition() :
        id("id"),
        asset("asset"),
        volume("volume")
    {}

    AnyObjectDefinition Type<UniversalSound>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { asset },
                { volume }
            }
        };
    }

    auto Type<Arca::Index<Audio::UniversalSound>>::ToV8(v8::Isolate& isolate, const Arca::Index<Audio::UniversalSound>& value) -> v8::Local<V8T>
    {
        if (!value)
            return v8::Null(&isolate);

        const auto asset = value->Asset();
        return JavaScript::ToV8(
            isolate,
            UniversalSound
            {
                value.ID(),
                asset
                    ? AudioAsset
                    {
                        asset.ID(),
                        asset->Name()
                    }
                    : std::optional<AudioAsset>(),
                value->Volume()
            });
    }

    std::optional<CreateUniversalSound> Type<CreateUniversalSound>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<CreateUniversalSound, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto asset = definition.asset.AtmosValue();
                const auto volume = definition.volume.AtmosValue();
                return asset && volume
                    ? CreateUniversalSound
                    {
                        *asset, *volume
                    }
                    : std::optional<CreateUniversalSound>{};
            });
    }

    Type<CreateUniversalSound>::Definition::Definition() :
        asset("asset"),
        volume("volume")
    {}

    AnyObjectDefinition Type<CreateUniversalSound>::Definition::ToAny()
    {
        return
        {
            {
                { asset },
                { volume }
            }
        };
    }

    std::optional<Arca::Create<Audio::UniversalSound>> Type<Arca::Create<Audio::UniversalSound>>::FromV8(
        v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto userData = static_cast<UserData*>(isolate.GetData(0));
        const auto reliquary = userData->reliquary;

        const auto command = JavaScript::FromV8<CreateUniversalSound>(isolate, value);
        return command
            ? Arca::Create<Audio::UniversalSound>(
                command->asset ? reliquary->Find<Asset::Audio>(command->asset->id) : Arca::Index<Asset::Audio>(),
                command->volume)
            : std::optional<Arca::Create<Audio::UniversalSound>>{};
    }
}