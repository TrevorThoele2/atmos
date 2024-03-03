#include "Sound.h"

namespace Atmos::Audio
{
    void Sound::PostConstruct()
    {
        bounds = Find<Bounds>();
    }

    void Sound::Initialize(Asset&& asset)
    {
        bounds = Create<Bounds>();
        this->asset = std::move(asset);
    }
}