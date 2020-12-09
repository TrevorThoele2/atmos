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

        Point<> Start();
        Duration<> Calculate();

        void ResetAverage();
        void ResetHighest();

        [[nodiscard]] Duration<> Elapsed() const;
        [[nodiscard]] bool IsStarted() const;
        [[nodiscard]] Point<> CurrentTime() const;

        [[nodiscard]] Duration<> Average() const;
        [[nodiscard]] Duration<> Highest() const;
    private:
        Point<> start;
        TimeRetriever timeRetriever;

        Nanoseconds average{};
        Nanoseconds highest{};
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
        template<class Archive>
        void Scriven(ObjectT&, Archive&)
        {}
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Time::Stopwatch, Archive> final
    {
        using Category = CompositeScribeCategory<Atmos::Time::Stopwatch>;
    };
}