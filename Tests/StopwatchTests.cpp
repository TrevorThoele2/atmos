#include <catch.hpp>

#include "StopwatchTests.h"

#include <thread>

using namespace Atmos;
using namespace Time;

SCENARIO_METHOD(StopwatchTestsFixture, "real stopwatch", "[stopwatch][time]")
{
    GIVEN("real stopwatch")
    {
        auto stopwatch = CreateRealStopwatch();

        WHEN("querying is started")
        {
            THEN("is not started")
            {
                REQUIRE(!stopwatch.IsStarted());
            }
        }

        WHEN("querying elapsed")
        {
            THEN("elapsed is zero")
            {
                const auto zero = Time::Duration<>();

                REQUIRE(stopwatch.Elapsed() == zero);
            }
        }

        WHEN("starting")
        {
            auto startTime = stopwatch.Start();

            const auto waitFor = Milliseconds(10);

            THEN("is started")
            {
                REQUIRE(stopwatch.IsStarted());
            }

            THEN("start is greater than zero")
            {
                const auto zero = Time::Value<>();

                REQUIRE(startTime > zero);
            }

            THEN("elapsed is greater than the waited time")
            {
                std::this_thread::sleep_for(waitFor);

                auto elapsedTime = stopwatch.Elapsed();

                REQUIRE(elapsedTime > waitFor);
            }

            WHEN("waiting then starting again")
            {
                std::this_thread::sleep_for(waitFor);

                auto startTime2 = stopwatch.Start();

                THEN("is started")
                {
                    REQUIRE(stopwatch.IsStarted());
                }

                THEN("start is greater than zero")
                {
                    const auto zero = Time::Value<>();

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