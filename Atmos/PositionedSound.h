#pragma once

#include <Arca/Relic.h>

#include "Sound.h"
#include "Bounds.h"

namespace Atmos::Audio
{
    class PositionedSound final : public Sound
    {
    public:
        PositionedSound(
            Arca::RelicInit init,
            Arca::Index<Asset::Audio> asset,
            Audio::Volume volume,
            Spatial::Point3D position);
        PositionedSound(Arca::RelicInit init, Arca::Serialization serialization);
        PositionedSound(PositionedSound&& arg) noexcept;

        PositionedSound& operator=(PositionedSound&& arg) noexcept;
    private:
        Arca::Index<Spatial::Bounds> bounds;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Audio::PositionedSound>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static TypeName TypeName() { return "Atmos::Audio::PositionedSound"; }
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Audio::PositionedSound> final
    {
    public:
        using ObjectT = Atmos::Audio::PositionedSound;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            BaseScriven<Atmos::Audio::Sound>(object, archive);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Audio::PositionedSound, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Audio::PositionedSound>;
    };
}