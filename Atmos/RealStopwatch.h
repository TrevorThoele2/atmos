#pragma once

#include <Arca/OpenTypedRelic.h>

#include "StopwatchCore.h"

namespace Atmos::Time
{
    class RealStopwatch final : public Arca::OpenTypedRelic<RealStopwatch>
    {
    public:
        explicit RealStopwatch(Init init);

        Value Start() const;
        Value Elapsed() const;
        [[nodiscard]] Value CurrentTime() const;
    private:
        using Core = StopwatchCore;
        Arca::Index<Core> core{};
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