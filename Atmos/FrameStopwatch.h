#pragma once

#include <Arca/ClosedTypedRelicAutomation.h>
#include "Stopwatch.h"
#include "TimeInformation.h"

namespace Arca
{
    class Reliquary;
}

namespace Atmos::Time
{
    class FrameStopwatch final : public Arca::ClosedTypedRelicAutomation<FrameStopwatch>, public Stopwatch
    {
    public:
        bool operator==(const FrameStopwatch& arg) const;
        bool operator!=(const FrameStopwatch& arg) const;
    public:
        [[nodiscard]] Value CurrentTime() const override;
    public:
        void PostConstruct();
    private:
        Arca::Ptr<Information> timeInformation;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Time::FrameStopwatch>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static const TypeName typeName;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Time::FrameStopwatch, BinaryArchive> final :
        public ArcaCompositeScribe<::Atmos::Time::FrameStopwatch, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}