#include <catch.hpp>

#include "FrameTests.h"

#include <Atmos/FrameInformation.h>
#include <Atmos/TypeRegistration.h>
#include <Atmos/Work.h>

#include "DerivedEngine.h"
#include <thread>

void FrameTestsFixture::WaitCurator::Handle(const Work& command)
{
    std::this_thread::sleep_for(Time::Milliseconds(10));
}

SCENARIO_METHOD(FrameTestsFixture, "frame", "[frame]")
{
    GIVEN("setup engine with field")
    {
        Logging::Logger logger(Logging::Severity::Verbose);
        DerivedEngine engine(logger);
        engine.Setup();

        auto fieldOrigin = Arca::ReliquaryOrigin();
        RegisterFieldTypes(
            fieldOrigin,
            *engine.mockImageAssetManager,
            *engine.nullAudioManager,
            *engine.mockInputManager,
            *engine.mockGraphicsManager,
            *engine.mockScriptManager,
            Spatial::ScreenSize{
                std::numeric_limits<Spatial::ScreenSize::Dimension>::max(),
                std::numeric_limits<Spatial::ScreenSize::Dimension>::max() },
                *engine.mockWindow,
                engine.Logger());
        fieldOrigin.Register<WaitCurator>();
        auto pipeline = Arca::Pipeline();
        pipeline.push_back(StartStage());
        Arca::Stage stage;
        stage.Add<WaitCurator>();
        pipeline.push_back(stage);
        pipeline.push_back(EndStage());
        fieldOrigin.CuratorCommandPipeline<Work>(pipeline);
        World::Field field(0, fieldOrigin.Actualize());

        auto& fieldReliquary = field.Reliquary();

        engine.mockGraphicsManager->Initialize();

        WHEN("working field reliquary")
        {
            fieldReliquary.Do(Work{});

            auto& information = *Arca::Index<Information>(fieldReliquary);

            THEN("last elapsed is positive")
            {
                REQUIRE(information.lastElapsed.count() > 0);
            }

            THEN("total elapsed is positive")
            {
                REQUIRE(information.totalElapsed.count() > 0);
            }
        }
    }
}