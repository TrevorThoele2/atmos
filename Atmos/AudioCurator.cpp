#include "AudioCurator.h"

namespace Atmos::Audio
{
    AudioCurator::AudioCurator(Init init, AudioManager& manager) : Curator(init), manager(&manager)
    {}

    std::unique_ptr<Asset::Resource::Audio> AudioCurator::Handle(const Asset::Resource::Create<Asset::Resource::Audio>& command)
    {
        return manager->CreateAudioResource(command.buffer, command.name);
    }
}