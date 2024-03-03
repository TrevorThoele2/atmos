#include "Stopwatch.h"
#include <utility>

namespace Atmos::Time
{
    Stopwatch::Stopwatch(TimeRetriever timeRetriever) : timeRetriever(std::move(timeRetriever))
    {}

    Point<> Stopwatch::Start()
    {
        start = CurrentTime();
        return start;
    }

    Duration<> Stopwatch::Calculate()
    {
        if (!IsStarted())
            return {};

        const auto elapsed = Elapsed();

        if (elapsed > highest)
            highest = elapsed;

        // accumulator = (alpha * new_value) + (1.0 - alpha) * accumulator
        const auto alpha = Nanoseconds(1'000'000);
        average =
            (alpha * elapsed.count())
            + (Nanoseconds(1'000'000'000) - alpha)
            * average.count();

        return CurrentTime() - start;
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
        if (!IsStarted())
            return {};

        return CurrentTime() - start;
    }

    bool Stopwatch::IsStarted() const
    {
        return start != Point<>();
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
}