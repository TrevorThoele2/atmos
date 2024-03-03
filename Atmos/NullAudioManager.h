#pragma once

#include "AudioManager.h"

namespace Atmos::Audio
{
    class NullManager final : public Manager
    {
    public:
        NullManager(Logging::Logger& logger);
        NullManager(const NullManager& arg) = delete;
        NullManager& operator=(const NullManager& arg) = delete;

        std::unique_ptr<Asset::Resource::Audio> CreateAssetResource(const Buffer& buffer, const Name& name) override;
        std::unique_ptr<Resource::Sound> CreateSoundResource(const Asset::Resource::Audio& asset, Volume volume) override;

        void DestroyingSoundResource(Resource::Sound& resource) override;

        void SetMasterVolume(Volume volume) override;

        void PruneDoneResources() override;
        [[nodiscard]] std::vector<Resource::Sound*> DoneResources() override;
    };
}