#include <catch.hpp>

#include "AngelScriptAudioTests.h"

#include "ScriptEngine.h"

#include <Atmos/UniversalSound.h>
#include <Atmos/PositionedSound.h>
#include <Atmos/TypeRegistration.h>
#include <Atmos/Script.h>
#include <Atmos/ScriptFinished.h>
#include <Atmos/Work.h>
#include <Atmos/StringUtility.h>
#include <Arca/LocalRelic.h>

#include "MockSoundResource.h"

using namespace Atmos::Audio;

Arca::Index<Atmos::Asset::Audio> AngelScriptAudioTestsFixture::CreateAudioAsset(Arca::Reliquary& reliquary)
{
    const auto audioAssetName = dataGeneration.Random<std::string>();

    auto resource = reliquary.Do(Asset::Resource::Create<Asset::Resource::Audio>{
        Buffer{}, audioAssetName });
    return reliquary.Do(Arca::Create<Asset::Audio>{
        audioAssetName, std::move(resource) });
}

SCENARIO_METHOD(AngelScriptAudioTestsFixture, "running audio AngelScript scripts", "[script][angelscript]")
{
    Logging::Logger logger(Logging::Severity::Verbose);
    logger.Add<Logging::FileSink>();
    ScriptEngine engine(logger);

    auto fieldOrigin = Arca::ReliquaryOrigin();
    fieldOrigin.Register<Arca::OpenRelic>();
    RegisterFieldTypes(
        fieldOrigin,
        *engine.mockImageAssetManager,
        *engine.mockAudioManager,
        *engine.mockInputManager,
        *engine.mockGraphicsManager,
        *engine.scriptManager,
        *engine.mockWorldManager,
        Spatial::ScreenSize{
            std::numeric_limits<Spatial::ScreenSize::Dimension>::max(),
            std::numeric_limits<Spatial::ScreenSize::Dimension>::max() },
            *engine.mockWindow,
            engine.Logger());
    fieldOrigin.CuratorCommandPipeline<Work>(Arca::Pipeline{ Audio::Stage(), Scripting::Stage() });
    World::Field field(0, fieldOrigin.Actualize());

    auto& fieldReliquary = field.Reliquary();

    std::vector<Scripting::Finished> finishes;
    fieldReliquary.On<Scripting::Finished>([&finishes](const Scripting::Finished& signal)
        {
            finishes.push_back(signal);
        });

    GIVEN("PositionedSound")
    {
        const auto audioAsset = CreateAudioAsset(fieldReliquary);
        const auto volume = dataGeneration.Random<Volume>();
        const auto position = dataGeneration.RandomStack<
            Spatial::Point3D, Spatial::Point3D::Value, Spatial::Point3D::Value, Spatial::Point3D::Value>();

        auto createCommand = Arca::Create<PositionedSound>{ audioAsset, volume, position };
        auto positionedSound = fieldReliquary.Do(createCommand);

        GIVEN("script that returns asset ID")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "Arca::RelicID main(Arca::RelicID id)\n" \
                "{\n" \
                "    auto sound = Atmos::Audio::PositionedSound(id);\n" \
                "    return sound.Asset().ID();\n" \
                "}",
                { positionedSound.ID() },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);
                    REQUIRE(std::get<Arca::RelicID>(std::get<Variant>(finishes[0].result)) == audioAsset.ID());
                }
            }
        }

        GIVEN("script that returns volume")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "float main(Arca::RelicID id)\n" \
                "{\n" \
                "    auto sound = Atmos::Audio::PositionedSound(id);\n" \
                "    return sound.Volume();\n" \
                "}",
                { positionedSound.ID() },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);
                    REQUIRE(std::get<Audio::Volume>(std::get<Variant>(finishes[0].result)) == positionedSound->Volume());
                }
            }
        }

        GIVEN("script that changes sound volume")
        {
            const auto newVolume = dataGeneration.Random<Audio::Volume>();

            CompileAndCreateScript(
                "basic_script.as",
                "void main(Arca::RelicID id, float newVolume)\n" \
                "{\n" \
                "    Arca::Reliquary::Do(Atmos::Audio::ChangeSoundVolume(id, newVolume));\n" \
                "}",
                { positionedSound.ID(), newVolume },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has paused sound")
                {
                    REQUIRE(positionedSound->ResourceAs<MockSoundResource>()->setVolumeCount == 1);
                }
            }
        }

        GIVEN("script that restarts sound")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "void main(Arca::RelicID id)\n" \
                "{\n" \
                "    Arca::Reliquary::Do(Atmos::Audio::RestartSound(id));\n" \
                "}",
                { positionedSound.ID() },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has restarted sound")
                {
                    REQUIRE(positionedSound->ResourceAs<MockSoundResource>()->restartCount == 1);
                }
            }
        }

        GIVEN("script that pauses sound")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "void main(Arca::RelicID id)\n" \
                "{\n" \
                "    Arca::Reliquary::Do(Atmos::Audio::PauseSound(id));\n" \
                "}",
                { positionedSound.ID() },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has paused sound")
                {
                    REQUIRE(positionedSound->ResourceAs<MockSoundResource>()->pauseCount == 1);
                }
            }
        }

        GIVEN("script that resumes sound")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "void main(Arca::RelicID id)\n" \
                "{\n" \
                "    Arca::Reliquary::Do(Atmos::Audio::ResumeSound(id));\n" \
                "}",
                { positionedSound.ID() },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has paused sound")
                {
                    REQUIRE(positionedSound->ResourceAs<MockSoundResource>()->resumeCount == 1);
                }
            }
        }

        GIVEN("script that listens for sound finished")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "class SignalHandler\n" \
                "{\n" \
                "    Atmos::Audio::SoundFinished signal;\n" \
                "    void Handle(Atmos::Audio::SoundFinished signal) { this.signal = signal; }\n" \
                "}\n" \
                "\n" \
                "Arca::RelicID main()\n" \
                "{\n" \
                "    SignalHandler signalHandler;\n" \
                "    Arca::Reliquary::On(Atmos::Audio::OnSoundFinished(signalHandler.Handle));\n" \
                "\n" \
                "    Arca::Reliquary::Do(Atmos::Scripting::Suspend(Atmos::CurrentScript()));\n" \
                "\n" \
                "    return signalHandler.signal.id;\n" \
                "}",
                {},
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});
                auto resource = const_cast<Audio::Resource::Sound*>(positionedSound->Resource());
                engine.mockAudioManager->doneResources.push_back(resource);
                fieldReliquary.Do(Work{});

                THEN("has paused sound")
                {
                    REQUIRE(finishes.size() == 1);
                    REQUIRE(std::get<Arca::RelicID>(std::get<Variant>(finishes[0].result)) == positionedSound.ID());
                }
            }
        }
    }

    GIVEN("UniversalSound")
    {
        const auto audioAsset = CreateAudioAsset(fieldReliquary);
        const auto volume = dataGeneration.Random<Volume>();

        auto createCommand = Arca::Create<UniversalSound>{ audioAsset, volume };
        auto universalSound = fieldReliquary.Do(createCommand);

        GIVEN("script that returns asset ID")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "Arca::RelicID main(Arca::RelicID id)\n" \
                "{\n" \
                "    auto sound = Atmos::Audio::UniversalSound(id);\n" \
                "    return sound.Asset().ID();\n" \
                "}",
                { universalSound.ID() },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);
                    REQUIRE(std::get<Arca::RelicID>(std::get<Variant>(finishes[0].result)) == audioAsset.ID());
                }
            }
        }

        GIVEN("script that returns volume")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "float main(Arca::RelicID id)\n" \
                "{\n" \
                "    auto sound = Atmos::Audio::UniversalSound(id);\n" \
                "    return sound.Volume();\n" \
                "}",
                { universalSound.ID() },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);
                    REQUIRE(std::get<Audio::Volume>(std::get<Variant>(finishes[0].result)) == universalSound->Volume());
                }
            }
        }
    }
}