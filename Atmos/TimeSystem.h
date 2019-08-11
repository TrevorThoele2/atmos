#pragma once

#include <chrono>

#include "ObjectSystem.h"

#include "TimeValue.h"

namespace Atmos::Time
{
    class TimeSystem : public ObjectSystem
    {
    public:
        TimeSystem(ObjectManager& manager);

        void OnFrameEnd();
        Value GetTotalElapsed() const;
        Value GetLastElapsed() const;

        // Returns the exact time of the system
        Value CurrentTime() const;
    private:
        std::chrono::high_resolution_clock clock;

        Value totalElapsed;
        Value lastElapsed;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Time::TimeSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::Time::TimeSystem, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}