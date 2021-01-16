#include "AudioAssetCurator.h"

#include <Chroma/Overloaded.h>

namespace Atmos::Asset
{
    AudioCurator::AudioCurator(Init init, Resource::Manager& resourceManager) :
        Curator(init), resourceManager(&resourceManager)
    {}

    Resource::LoadedData<Resource::Audio> AudioCurator::Handle(const Resource::LoadData<Resource::Audio>& command)
    {
        return std::visit(Chroma::Overloaded{
            [this](const File::Path& filePath) -> Resource::LoadedData<Resource::Audio>
            {
                return resourceManager->LoadAudioData(filePath);
            },
            [this](const Buffer& memory) -> Resource::LoadedData<Resource::Audio>
            {
                return resourceManager->LoadAudioData(memory);
            } },
            command.from);
    }
}