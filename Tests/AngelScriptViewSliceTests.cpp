#include <catch.hpp>

#include "AngelScriptViewSliceTests.h"

#include "ScriptEngine.h"

#include <Atmos/ViewSlice.h>
#include <Atmos/Camera.h>
#include <Atmos/TypeRegistration.h>
#include <Atmos/Script.h>
#include <Atmos/ScriptFinished.h>
#include <Atmos/Work.h>
#include <Arca/OpenRelic.h>

SCENARIO_METHOD(AngelScriptViewSliceTestsFixture, "running view slice AngelScript scripts", "[script][angelscript]")
{
    Logging::Logger logger(Logging::Severity::Verbose);
    logger.Add<Logging::FileSink>();
    ScriptEngine engine(logger);

    auto fieldOrigin = Arca::ReliquaryOrigin();
    fieldOrigin.Register<Arca::OpenRelic>();
    RegisterFieldTypes(
        fieldOrigin,
        *engine.mockAssetResourceManager,
        *engine.mockAudioManager,
        *engine.mockInputManager,
        *engine.mockGraphicsManager,
        *engine.mockTextManager,
        *engine.scriptManager,
        *engine.mockWorldManager,
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

    GIVEN("ViewSlice")
    {
        auto relic = fieldReliquary.Do(Arca::Create<Arca::OpenRelic>());
        auto viewSlice = fieldReliquary.Do(Arca::Create<ViewSlice>{ relic.ID(), Spatial::AxisAlignedBox2D{} });

        GIVEN("box")
        {
            auto box = Spatial::AxisAlignedBox2D
            {
                dataGeneration.RandomStack<Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>(),
                dataGeneration.RandomStack<Spatial::Size2D, Spatial::Size2D::Value, Spatial::Size2D::Value>()
            };

            GIVEN("script that sets box and returns box")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "string main(Arca::RelicID id, float centerX, float centerY, float width, float height)\n" \
                    "{\n" \
                    "    Atmos::Spatial::AxisAlignedBox2D to(Atmos::Spatial::Point2D(centerX, centerY), Atmos::Spatial::Size2D(width, height));\n" \
                    "    Arca::Reliquary::Do(Atmos::Render::ChangeViewSliceBox(id, to));\n" \
                    "\n" \
                    "    auto viewSlice = Atmos::Render::ViewSlice(id);\n" \
                    "    return \n" \
                    "        Atmos::ToString(viewSlice.Box().center.x) + \" \" +\n" \
                    "        Atmos::ToString(viewSlice.Box().center.y) + \" \" +\n" \
                    "        Atmos::ToString(viewSlice.Box().size.width) + \" \" +\n" \
                    "        Atmos::ToString(viewSlice.Box().size.height);\n" \
                    "}",
                    { viewSlice.ID(), box.center.x, box.center.y, box.size.width, box.size.height },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);

                        const auto expectedResult =
                            ToString(viewSlice->box.center.x) + " " +
                            ToString(viewSlice->box.center.y) + " " +
                            ToString(viewSlice->box.size.width) + " " +
                            ToString(viewSlice->box.size.height);
                        REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == expectedResult);
                    }
                }
            }
        }
    }
}