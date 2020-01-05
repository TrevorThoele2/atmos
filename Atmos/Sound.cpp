#include "Sound.h"

namespace Atmos::Audio
{
    void Sound::PostConstruct(ShardTuple shards)
    {
        bounds = std::get<0>(shards);
    }

    void Sound::Initialize(Asset&& asset)
    {
        this->asset = std::move(asset);
    }
}