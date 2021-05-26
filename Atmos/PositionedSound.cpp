#include "PositionedSound.h"

namespace Atmos::Audio
{
    PositionedSound::PositionedSound(
        Arca::RelicInit init,
        Arca::Index<Asset::Audio> asset,
        Audio::Volume volume,
        Spatial::Point3D position)
        :
        Sound(init, asset, volume)
    {
        bounds = init.Create<Spatial::Bounds>(
            Spatial::BoundsSpace::World, position, Spatial::Size2D{ 0, 0 }, Spatial::Scalers2D{ 0, 0 }, 0.0f);
    }

    PositionedSound::PositionedSound(Arca::RelicInit init, Arca::Serialization serialization) :
        Sound(init, serialization),
        bounds(init.Find<Spatial::Bounds>())
    {}

    PositionedSound::PositionedSound(PositionedSound&& arg) noexcept :
        Sound(std::move(arg)),
        bounds(std::move(arg.bounds))
    {}

    PositionedSound& PositionedSound::operator=(PositionedSound&& arg) noexcept
    {
        Sound::operator=(std::move(arg));
        bounds = std::move(arg.bounds);
        return *this;
    }
}