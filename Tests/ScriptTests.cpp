#include <catch.hpp>

#include "ScriptTests.h"

#include "DerivedEngine.h"

#include <Atmos/TypeRegistration.h>
#include <Atmos/Script.h>
#include <Atmos/Work.h>
#include <Atmos/ScriptFinished.h>
#include <Arca/Create.h>

SCENARIO_METHOD(ScriptTestsFixture, "script tests", "[script]")
{
    Logging::Logger logger(Logging::Severity::Verbose);
    logger.Add<Logging::FileSink>();
    DerivedEngine engine(logger);

    auto fieldOrigin = Arca::ReliquaryOrigin();
    RegisterFieldTypes(
        fieldOrigin,
        *engine.mockAssetResourceManager,
        *engine.mockAudioManager,
        *engine.mockInputManager,
        *engine.mockGraphicsManager,
        *engine.mockTextManager,
        *engine.mockScriptManager,
        *engine.worldManager,
        Spatial::Size2D{
            std::numeric_limits<Spatial::Size2D::Value>::max(),
            std::numeric_limits<Spatial::Size2D::Value>::max() },
            *engine.mockWindow,
            engine.Logger());
    fieldOrigin.CuratorCommandPipeline<Work>(Arca::Pipeline{ Scripting::Stage() });
    World::Field field(0, fieldOrigin.Actualize());

    auto& fieldReliquary = field.Reliquary();

    std::vector<Scripting::Finished> finishes;
    fieldReliquary.On<Scripting::Finished>([&finishes](const Scripting::Finished& signal)
        {
            finishes.push_back(signal);
        });

    GIVEN("script without asset")
    {
        auto script = fieldReliquary.Do(Arca::Create<Scripting::Script>{
            Arca::Index<Asset::Script>{}, "", Scripting::Parameters{}});

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("sends a quit result")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::holds_alternative<Scripting::Quit>(finishes[0].result));
                REQUIRE(finishes[0].script == script);
            }

            THEN("destroys script")
            {
                auto checkScript = fieldReliquary.Find<Scripting::Script>(script.ID());
                REQUIRE(!checkScript);
            }
        }
    }
}