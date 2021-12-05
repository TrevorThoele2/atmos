#pragma once

#include "ArcaRelicIncludes.h"

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

        bool operator==(const UniversalSound& arg) const;
        bool operator!=(const UniversalSound& arg) const;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Audio::UniversalSound>
    {
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::Audio::UniversalSound";
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
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            BaseScriven<Atmos::Audio::Sound>(object, format);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Audio::UniversalSound, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Audio::UniversalSound>;
    };
}