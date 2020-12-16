#include <catch.hpp>

#include "StopwatchTests.h"

#include <thread>

using namespace Atmos;
using namespace Time;

SCENARIO_METHOD(StopwatchTestsFixture, "real stopwatch", "[time][stopwatch]")
{
    GIVEN("real stopwatch")
    {
        auto stopwatch = CreateRealStopwatch();

        WHEN("querying elapsed")
        {
            THEN("elapsed is not zero")
            {
                const auto zero = Time::Duration<>();

                REQUIRE(stopwatch.Elapsed() != zero);
            }
        }

        WHEN("restarting")
        {
            auto startTime = stopwatch.Restart();

            const auto waitFor = Milliseconds(10);

            THEN("start is greater than zero")
            {
                const auto zero = Time::Point<>();

                REQUIRE(startTime > zero);
            }

            THEN("elapsed is greater than the waited time")
            {
                std::this_thread::sleep_for(waitFor);

                auto elapsedTime = stopwatch.Elapsed();

                REQUIRE(elapsedTime > waitFor);
            }

            WHEN("waiting then restarting again")
            {
                std::this_thread::sleep_for(waitFor);

                auto startTime2 = stopwatch.Restart();

                THEN("start is greater than zero")
                {
                    const auto zero = Time::Point<>();

                    REQUIRE(startTime > zero);
                }

                THEN("second start is greater than first start")
                {
                    REQUIRE(startTime2 > startTime);
                }

                THEN("elapsed is greater than the waited time")
                {
                    std::this_thread::sleep_for(waitFor);

                    auto elapsedTime = stopwatch.Elapsed();

                    REQUIRE(elapsedTime > waitFor);
                }
            }
        }
    }
}