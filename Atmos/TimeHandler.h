#pragma once

#include <chrono>
#include "TimeValue.h"

namespace Atmos
{
    class TimeHandler
    {
    private:
        std::chrono::high_resolution_clock clock;

        TimeValue totalElapsed;
        TimeValue lastElapsed;

        TimeHandler(const TimeHandler &arg) = delete;
        TimeHandler& operator=(const TimeHandler &arg) = delete;
    public:
        TimeHandler() = default;
        void OnFrameEnd();
        TimeValue GetTotalElapsed() const;
        TimeValue GetLastElapsed() const;

        // Returns the exact time of the system
        TimeValue CurrentTime() const;
    };
}