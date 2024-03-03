#pragma once

#include <chrono>

#include "ObjectSystem.h"

#include "TimeValue.h"

namespace Atmos
{
    class TimeSystem : public ObjectSystem
    {
    public:
        TimeSystem(ObjectManager& manager);
        TimeSystem(const ::Inscription::Table<TimeSystem>& table);

        void OnFrameEnd();
        TimeValue GetTotalElapsed() const;
        TimeValue GetLastElapsed() const;

        // Returns the exact time of the system
        TimeValue CurrentTime() const;
    private:
        std::chrono::high_resolution_clock clock;

        TimeValue totalElapsed;
        TimeValue lastElapsed;
    private:

    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::TimeSystem)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}