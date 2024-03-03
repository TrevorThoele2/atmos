#include "Stopwatch.h"
#include <utility>

namespace Atmos::Time
{
    Stopwatch::Stopwatch(TimeRetriever timeRetriever) : timeRetriever(std::move(timeRetriever))
    {
        start = CurrentTime();
    }

    void Stopwatch::Pause()
    {
        if (!isPaused)
        {
            storedDuration += CurrentLegElapsed();
            isPaused = true;
        }
    }

    Point<> Stopwatch::Resume()
    {
        if (isPaused)
        {
            start = CurrentTime();
            isPaused = false;
            return start;
        }

        return {};
    }

    Point<> Stopwatch::Restart()
    {
        start = CurrentTime();
        storedDuration = Duration<>::zero();
        isPaused = false;
        return start;
    }

    Duration<> Stopwatch::Calculate()
    {
        const auto elapsed = Elapsed();

        if (elapsed > highest)
            highest = elapsed;

        // accumulator = (alpha * new_value) + (1.0 - alpha) * accumulator
        const auto alpha = Nanoseconds(1'000'000);
        average =
            (alpha * elapsed.count())
            + (Nanoseconds(1'000'000'000) - alpha)
            * average.count();

        return elapsed;
    }

    void Stopwatch::ResetAverage()
    {
        average = Nanoseconds{};
    }

    void Stopwatch::ResetHighest()
    {
        highest = Nanoseconds{};
    }

    Duration<> Stopwatch::Elapsed() const
    {
        return !isPaused ? CurrentLegElapsed() + storedDuration : storedDuration;
    }

    Point<> Stopwatch::CurrentTime() const
    {
        return timeRetriever();
    }

    Duration<> Stopwatch::Average() const
    {
        return average;
    }

    Duration<> Stopwatch::Highest() const
    {
        return highest;
    }

    Duration<> Stopwatch::CurrentLegElapsed() const
    {
        const auto currentTime = CurrentTime();
        return currentTime - start;
    }
}