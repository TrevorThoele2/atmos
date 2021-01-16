#pragma once

#include <Arca/Relic.h>

#include "Sound.h"

namespace Atmos::Audio
{
    class UniversalSound final : public Sound
    {
    public:
        UniversalSound(
            Arca::RelicInit init,
            Arca::Index<Asset::Audio> asset,
            Audio::Volume volume);
        UniversalSound(Arca::RelicInit init, Arca::Serialization serialization);
        UniversalSound(UniversalSound&& arg) noexcept;

        UniversalSound& operator=(UniversalSound&& arg) noexcept;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Audio::UniversalSound>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static TypeName TypeName() { return "Atmos::Audio::UniversalSound"; }
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Audio::UniversalSound> final
    {
    public:
        using ObjectT = Atmos::Audio::UniversalSound;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            BaseScriven<Atmos::Audio::Sound>(object, archive);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Audio::UniversalSound, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Audio::UniversalSound>;
    };
}