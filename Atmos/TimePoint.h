#pragma once

#include <chrono>

#include "Serialization.h"

namespace Atmos::Time
{
    using Hours = std::chrono::duration<long long, std::ratio<3600>>;
    using Minutes = std::chrono::duration<long long, std::ratio<60>>;
    using Seconds = std::chrono::seconds;
    using Milliseconds = std::chrono::milliseconds;
    using Microseconds = std::chrono::microseconds;
    using Nanoseconds = std::chrono::nanoseconds;

    using Atto = std::atto;
    using Femto = std::femto;
    using Pico = std::pico;
    using Nano = std::nano;
    using Micro = std::micro;
    using Milli = std::milli;
    using Centi = std::centi;
    using Deci = std::deci;
    using Deca = std::deca;
    using Hecto = std::hecto;
    using Kilo = std::kilo;
    using Mega = std::mega;
    using Giga = std::giga;
    using Tera = std::tera;
    using Peta = std::peta;
    using Exa = std::exa;

    template<class Units = Nano>
    using Duration = std::chrono::duration<long long, Units>;

    template<class Units = Nanoseconds>
    using Point = std::chrono::time_point<std::chrono::steady_clock, Units>;
}

namespace Inscription
{
    template<class Clock, class Duration>
    class Scribe<std::chrono::time_point<Clock, Duration>> final
    {
    public:
        using ObjectT = std::chrono::time_point<Clock, Duration>;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            if (format.IsOutput())
            {
                auto time = object.time_since_epoch().count();
                format(time);
            }
            else
            {
                long long time;
                format(time);

                object = ObjectT(ObjectT::duration(time));
            }
        }
    };

    template<class Clock, class Duration, class Format>
    struct ScribeTraits<std::chrono::time_point<Clock, Duration>, Format>
    {
        using Category = CompositeScribeCategory<std::chrono::time_point<Clock, Duration>>;
    };
}