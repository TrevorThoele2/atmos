#include "SoundResource.h"

namespace Atmos::Audio::Resource
{
    Sound::~Sound() = default;

    const Spatial::Point2D::Value Sound::maxDistance = 2048.0f;
}