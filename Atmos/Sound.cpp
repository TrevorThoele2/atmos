#include "Sound.h"

namespace Atmos::Audio
{
    void Sound::PostConstruct()
    {
        bounds = Find<Spatial::Bounds>();
    }

    void Sound::Initialize(Asset&& asset)
    {
        bounds = Create<Spatial::Bounds>();
        this->asset = std::move(asset);
    }
}