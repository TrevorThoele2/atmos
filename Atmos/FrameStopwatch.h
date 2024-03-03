#pragma once

#include <Arca/OpenTypedRelic.h>

#include "StopwatchCore.h"
#include "TimeInformation.h"

namespace Atmos::Time
{
    class FrameStopwatch final : public Arca::OpenTypedRelic<FrameStopwatch>
    {
    public:
        explicit FrameStopwatch(Init init);

        Value Start() const;
        [[nodiscard]] Value Elapsed() const;
        [[nodiscard]] Value CurrentTime() const;
    private:
        using Core = StopwatchCore;
        Arca::ShardIndex<Core> core{};

        Arca::GlobalIndex<Information> timeInformation{};
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
        static inline const TypeName typeName = "FrameStopwatch";
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