#pragma once

#include <chrono>

#include "Serialization.h"

namespace Atmos::Time
{
    using Hours = std::chrono::hours;
    using Minutes = std::chrono::minutes;
    using Seconds = std::chrono::seconds;
    using Milliseconds = std::chrono::milliseconds;
    using Microseconds = std::chrono::microseconds;
    using Nanoseconds = std::chrono::nanoseconds;

    template<class Rep = long long, class Period = std::nano>
    using Duration = std::chrono::duration<Rep, Period>;

    template<class Duration = Nanoseconds>
    using Value = std::chrono::time_point<std::chrono::steady_clock, Duration>;
}

namespace Inscription
{
    template<class Clock, class Duration>
    class Scribe<std::chrono::time_point<Clock, Duration>> final
    {
    public:
        using ObjectT = std::chrono::time_point<Clock, Duration>;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            if (archive.IsOutput())
            {
                auto time = object.time_since_epoch().count();
                archive(time);
            }
            else
            {
                long long time;
                archive(time);

                object = ObjectT(ObjectT::duration(time));
            }
        }
    };

    template<class Clock, class Duration, class Archive>
    struct ScribeTraits<std::chrono::time_point<Clock, Duration>, Archive>
    {
        using Category = CompositeScribeCategory<std::chrono::time_point<Clock, Duration>>;
    };
}