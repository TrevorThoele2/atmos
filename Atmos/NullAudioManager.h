#pragma once

#include "AudioManager.h"

namespace Atmos::Audio
{
    class NullAudioManager final : public AudioManager
    {
    public:
        NullAudioManager() = default;
        NullAudioManager(const NullAudioManager& arg) = delete;
        NullAudioManager& operator=(const NullAudioManager& arg) = delete;

        bool SetMasterVolume(float set) override;
    private:
        std::unique_ptr<Asset::Resource::Audio> CreateAudioResourceImpl(
            const FormattedBuffer& buffer, const Name& name) override;
    };
}