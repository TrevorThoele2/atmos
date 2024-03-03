#include <catch.hpp>

#include "StopwatchTests.h"
#include <Atmos/TypeRegistration.h>

#include <Arca/ReliquaryOrigin.h>

#include <thread>

using namespace Atmos;
using namespace Time;

SCENARIO_METHOD(StopwatchTestsFixture, "real stopwatch", "[stopwatch][time]")
{
    GIVEN("registered reliquary")
    {
        Arca::ReliquaryOrigin origin;
        Time::RegisterTypes(origin);
        auto reliquary = origin.Actualize();

        WHEN("creating real stopwatch")
        {
            const auto stopwatch = reliquary->Do<Arca::Create<Time::RealStopwatch>>();

            WHEN("starting")
            {
                auto startTime = stopwatch->Start();

                THEN("start is greater than zero")
                {
                    const auto zero = Time::Value<>();
                    REQUIRE(startTime > zero);
                }

                THEN("elapsed is greater waited time")
                {
                    std::this_thread::sleep_for(Milliseconds(10));

                    auto calculatedTime = stopwatch->Elapsed();

                    const auto waitedFor = Milliseconds(10);
                    REQUIRE(calculatedTime > waitedFor);
                }
            }
        }
    }
}