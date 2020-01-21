#include "Sound.h"

namespace Atmos::Audio
{
    void Sound::PostConstruct()
    {
        bounds = Create<Bounds>();
    }

    void Sound::Initialize(Asset&& asset)
    {
        this->asset = std::move(asset);
    }
}