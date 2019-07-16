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
    template<>
    class Scribe<::Atmos::TimeSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::TimeSystem, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}