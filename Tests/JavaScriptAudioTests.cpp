#include <catch.hpp>

#include "JavaScriptAudioTests.h"

#include <Atmos/UniversalSound.h>
#include <Arca/Create.h>

SCENARIO_METHOD(JavaScriptAudioTestsFixture, "running audio JavaScript scripts", "[script][javascript][logging]")
{
    GIVEN("script that sends ChangeSoundVolume")
    {
        const auto audioAssetName = dataGeneration.Random<std::string>();

        auto resource = fieldReliquary->Do(Asset::Resource::Create<Asset::Resource::Audio>{
            Buffer{}, audioAssetName });
        auto asset = fieldReliquary->Do(Arca::Create<Asset::Audio>{
            audioAssetName, std::move(resource) });
        
        auto firstVolume = dataGeneration.Random<Audio::Volume>();
        auto secondVolume = dataGeneration.Random<Audio::Volume>();

        const auto sound = fieldReliquary->Do(Arca::Create<Audio::UniversalSound>{asset, firstVolume});

        CompileAndCreateScript(
            "basic_script.ts",
 R"V0G0N(import { Atmos } from "./atmos";

export const main = (id: Atmos.RelicId, volume: Atmos.Audio.Volume): Atmos.Result => {
    Atmos.Reliquary.send(
        Atmos.Traits.Audio.ChangeSoundVolume.typeName,
        {
            id,
            to: volume
        });
    return {
        done: true
    };
};)V0G0N",
            { sound.ID(), secondVolume },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(sound->Volume() == secondVolume);
            }
        }
    }

    GIVEN("script that sends PauseSound")
    {
        const auto audioAssetName = dataGeneration.Random<std::string>();

        auto resource = fieldReliquary->Do(Asset::Resource::Create<Asset::Resource::Audio>{
            Buffer{}, audioAssetName });
        auto asset = fieldReliquary->Do(Arca::Create<Asset::Audio>{
            audioAssetName, std::move(resource) });

        auto volume = dataGeneration.Random<Audio::Volume>();

        const auto sound = fieldReliquary->Do(Arca::Create<Audio::UniversalSound>{asset, volume});

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (id: Atmos.RelicId): Atmos.Result => {
    Atmos.Reliquary.send(
        Atmos.Traits.Audio.PauseSound.typeName,
        {
            id
        });
    return {
        done: true
    };
};)V0G0N",
            { sound.ID() },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
            }
        }
    }

    GIVEN("script that sends RestartSound")
    {
        const auto audioAssetName = dataGeneration.Random<std::string>();

        auto resource = fieldReliquary->Do(Asset::Resource::Create<Asset::Resource::Audio>{
            Buffer{}, audioAssetName });
        auto asset = fieldReliquary->Do(Arca::Create<Asset::Audio>{
            audioAssetName, std::move(resource) });

        auto volume = dataGeneration.Random<Audio::Volume>();

        const auto sound = fieldReliquary->Do(Arca::Create<Audio::UniversalSound>{asset, volume});

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (id: Atmos.RelicId): Atmos.Result => {
    Atmos.Reliquary.send(
        Atmos.Traits.Audio.RestartSound.typeName,
        {
            id
        });
    return {
        done: true
    };
};)V0G0N",
            { sound.ID() },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
            }
        }
    }

    GIVEN("script that sends ResumeSound")
    {
        const auto audioAssetName = dataGeneration.Random<std::string>();

        auto resource = fieldReliquary->Do(Asset::Resource::Create<Asset::Resource::Audio>{
            Buffer{}, audioAssetName });
        auto asset = fieldReliquary->Do(Arca::Create<Asset::Audio>{
            audioAssetName, std::move(resource) });

        auto volume = dataGeneration.Random<Audio::Volume>();

        const auto sound = fieldReliquary->Do(Arca::Create<Audio::UniversalSound>{asset, volume});

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (id: Atmos.RelicId): Atmos.Result => {
    Atmos.Reliquary.send(
        Atmos.Traits.Audio.ResumeSound.typeName,
        {
            id
        });
    return {
        done: true
    };
};)V0G0N",
            { sound.ID() },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);
            }
        }
    }

    GIVEN("script that listens for sound finished")
    {
        const auto audioAssetName = dataGeneration.Random<std::string>();

        auto resource = fieldReliquary->Do(Asset::Resource::Create<Asset::Resource::Audio>{
            Buffer{}, audioAssetName });
        auto asset = fieldReliquary->Do(Arca::Create<Asset::Audio>{
            audioAssetName, std::move(resource) });

        auto volume = dataGeneration.Random<Audio::Volume>();

        const auto sound = fieldReliquary->Do(Arca::Create<Audio::UniversalSound>{asset, volume});

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

let signal: Atmos.Audio.SoundFinished | null = null;
const onSignal = (receivedSignal: Atmos.Audio.SoundFinished) => {
    signal = receivedSignal;
}

export const main = (): Atmos.Result => {
    Atmos.Reliquary.listen(Atmos.Traits.Audio.SoundFinished.typeName, onSignal);
    return {
        done: signal !== null,
        value: signal !== null ? signal.id : null
    };
};)V0G0N",
            {},
            *fieldReliquary);

        WHEN("working reliquary")
        {
            auto resource = const_cast<Audio::Resource::Sound*>(sound->Resource());
            engine.mockAudioManager->doneResources.push_back(resource);
            fieldReliquary->Do(Work{});
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<Arca::RelicID>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == sound.ID());
            }
        }
    }
}