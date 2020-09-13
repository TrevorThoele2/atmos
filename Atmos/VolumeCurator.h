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
        Arca::Index<VolumeInformation> volumeInformation;
        Arca::Index<Initialization::Information> initializationInformation;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Audio::VolumeCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "Atmos::Audio::VolumeCurator";
        using HandledCommands = Arca::HandledCommands<Atmos::Audio::ChangeMasterVolume>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Audio::VolumeCurator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Audio::VolumeCurator>;
    };
}