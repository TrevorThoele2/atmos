#pragma once

#include <functional>
#include "TimePoint.h"

namespace Atmos::Time
{
    class Stopwatch
    {
    public:
        using TimeRetriever = std::function<Point<>()>;
    public:
        explicit Stopwatch(TimeRetriever timeRetriever);

        Stopwatch(const Stopwatch& arg) = default;
        Stopwatch(Stopwatch&& arg) noexcept = default;
        Stopwatch& operator=(const Stopwatch& arg) = default;
        Stopwatch& operator=(Stopwatch&& arg) noexcept = default;

        void Pause();
        Point<> Resume();
        Point<> Restart();
        Duration<> Calculate();

        void ResetAverage();
        void ResetHighest();

        [[nodiscard]] Duration<> Elapsed() const;
        [[nodiscard]] Point<> CurrentTime() const;

        [[nodiscard]] Duration<> Average() const;
        [[nodiscard]] Duration<> Highest() const;
    private:
        Point<> start;
        Duration<> storedDuration = Duration<>::zero();
        TimeRetriever timeRetriever;
        bool isPaused = false;

        Nanoseconds average{};
        Nanoseconds highest{};

        [[nodiscard]] Duration<> CurrentLegElapsed() const;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Time::Stopwatch> final
    {
    public:
        using ObjectT = Atmos::Time::Stopwatch;
    public:
        template<class Format>
        void Scriven(ObjectT&, Format&)
        {}
    };

    template<class Format>
    struct ScribeTraits<Atmos::Time::Stopwatch, Format> final
    {
        using Category = CompositeScribeCategory<Atmos::Time::Stopwatch>;
    };
}