#pragma once

#include <functional>
#include "TimeValue.h"

namespace Atmos::Time
{
    class Stopwatch
    {
    public:
        using TimeRetriever = std::function<Value<>()>;
    public:
        explicit Stopwatch(TimeRetriever timeRetriever);

        Value<> Start();
        Duration<> Calculate();

        void ResetAverage();
        void ResetHighest();

        [[nodiscard]] Duration<> Elapsed() const;
        [[nodiscard]] bool IsStarted() const;
        [[nodiscard]] Value<> CurrentTime() const;

        [[nodiscard]] Duration<> Average() const;
        [[nodiscard]] Duration<> Highest() const;
    private:
        Value<> start;
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