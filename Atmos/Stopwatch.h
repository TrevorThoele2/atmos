#pragma once

#include <Arca/ClosedTypedRelicAutomation.h>

#include "TimeValue.h"

namespace Atmos::Time
{
    class Stopwatch
    {
    public:
        virtual ~Stopwatch() = 0;

        bool operator==(const Stopwatch& arg) const;
        bool operator!=(const Stopwatch& arg) const;

        Value Start();
        [[nodiscard]] Value Elapsed() const;

        [[nodiscard]] virtual Value CurrentTime() const = 0;
    private:
        Value start;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Time::Stopwatch, BinaryArchive> final :
        public ArcaCompositeScribe<::Atmos::Time::Stopwatch, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}