#pragma once

#include <Arca/Curator.h>

#include "TimeInformation.h"
#include "TimeSettings.h"
#include "FrameStopwatch.h"

namespace Atmos::Time
{
    class TimeCurator final : public Arca::Curator
    {
    public:
        void StartFrame();
        // Returns if idle is still going
        bool DoIdle();
    protected:
        void InitializeImplementation() override;
    private:
        Arca::GlobalPtr<Information> timeInformation;
        Arca::GlobalPtr<Settings> timeSettings;

        using FpsStopwatch = ExtendedStopwatchAdapter<FrameStopwatch>;
        FpsStopwatch fpsTimer;
    private:
        [[nodiscard]] bool IsFrameDone() const;
        void FrameDone();
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Time::TimeCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "TimeCurator";
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Time::TimeCurator, BinaryArchive> final :
        public ArcaCompositeScribe<::Atmos::Time::TimeCurator, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}
