#include "JavaScriptPositionedSound.h"

#include "JavaScriptNumeric.h"
#include "JavaScriptAudioAsset.h"
#include "JavaScriptOptional.h"
#include "JavaScriptPoint3D.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<PositionedSound>::ToV8(v8::Isolate& isolate, const PositionedSound& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.asset.SetValue(isolate, value.asset);
        definition.volume.SetValue(isolate, value.volume);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<PositionedSound> Type<PositionedSound>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<PositionedSound, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                const auto asset = definition.asset.AtmosValue();
                const auto volume = definition.volume.AtmosValue();
                return id && asset && volume
                    ? PositionedSound
                    {
                        *id, *asset, *volume
                    }
                    : std::optional<PositionedSound>{};
            });
    }

    Type<PositionedSound>::Definition::Definition() :
        id("id"),
        asset("asset"),
        volume("volume")
    {}

    AnyObjectDefinition Type<PositionedSound>::Definition::ToAny()
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

    auto Type<Arca::Index<Audio::PositionedSound>>::ToV8(v8::Isolate& isolate, const Arca::Index<Audio::PositionedSound>& value) -> v8::Local<V8T>
    {
        if (!value)
            return v8::Null(&isolate);

        const auto asset = value->Asset();
        return JavaScript::ToV8(
            isolate,
            PositionedSound
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

    std::optional<CreatePositionedSound> Type<CreatePositionedSound>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<CreatePositionedSound, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto asset = definition.asset.AtmosValue();
                const auto volume = definition.volume.AtmosValue();
                const auto position = definition.position.AtmosValue();
                return asset && volume && position
                    ? CreatePositionedSound
                    {
                        *asset, *volume, *position
                    }
                    : std::optional<CreatePositionedSound>{};
            });
    }

    Type<CreatePositionedSound>::Definition::Definition() :
        asset("asset"),
        volume("volume"),
        position("position")
    {}

    AnyObjectDefinition Type<CreatePositionedSound>::Definition::ToAny()
    {
        return
        {
            {
                { asset },
                { volume },
                { position }
            }
        };
    }

    std::optional<Arca::Create<Audio::PositionedSound>> Type<Arca::Create<Audio::PositionedSound>>::FromV8(
        v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto userData = static_cast<UserData*>(isolate.GetData(0));
        const auto reliquary = userData->reliquary;

        const auto command = JavaScript::FromV8<CreatePositionedSound>(isolate, value);
        return command
            ? Arca::Create<Audio::PositionedSound>(
                command->asset ? reliquary->Find<Asset::Audio>(command->asset->id) : Arca::Index<Asset::Audio>(),
                command->volume,
                command->position)
            : std::optional<Arca::Create<Audio::PositionedSound>>{};
    }
}