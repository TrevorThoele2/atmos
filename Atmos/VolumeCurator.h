#pragma once

#include <Arca/Curator.h>

#include "VolumeInformation.h"
#include "ChangeMasterVolume.h"
#include "InitializationInformation.h"

#include "Serialization.h"

namespace Atmos::Audio
{
    class VolumeCurator final : public Arca::Curator
    {
    public:
        explicit VolumeCurator(Init init);
    public:
        void Handle(const ChangeMasterVolume& command);
    private:
        Arca::GlobalIndex<VolumeInformation> volumeInformation;
        Arca::GlobalIndex<Initialization::Information> initializationInformation;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Audio::VolumeCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "VolumeCurator";
        using HandledCommands = Arca::HandledCommands<Atmos::Audio::ChangeMasterVolume>;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Audio::VolumeCurator, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Audio::VolumeCurator, BinaryArchive>
    {};
}