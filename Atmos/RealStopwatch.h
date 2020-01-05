#pragma once

#include <Arca/ClosedTypedRelicAutomation.h>
#include "Stopwatch.h"

namespace Atmos::Time
{
    class RealStopwatch final : public Arca::ClosedTypedRelicAutomation<RealStopwatch>, public Stopwatch
    {
    public:
        bool operator==(const RealStopwatch& arg) const;

        [[nodiscard]] Value CurrentTime() const override;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Time::RealStopwatch>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "RealStopwatch";
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Time::RealStopwatch, BinaryArchive> final :
        public ArcaCompositeScribe<::Atmos::Time::RealStopwatch, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}