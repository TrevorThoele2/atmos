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
    class Scribe<::std::chrono::time_point<Clock, Duration>, BinaryArchive> final :
        public CompositeScribe<::std::chrono::time_point<Clock, Duration>, BinaryArchive>
    {
    private:
        using BaseT = CompositeScribe<::std::chrono::time_point<Clock, Duration>, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;

        using BaseT::Scriven;
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override
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
}