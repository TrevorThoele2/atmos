#pragma once

#include "AudioManager.h"

namespace Atmos::Audio
{
    class NullAudioManager : public AudioManager
    {
    public:
        NullAudioManager() = default;
        bool SetMasterVolume(float setTo) override;
    private:
        NullAudioManager(const NullAudioManager& arg) = delete;
        NullAudioManager& operator=(const NullAudioManager& arg) = delete;

        std::unique_ptr<Asset::AudioAssetData> CreateAudioDataImpl(
            ExtractedFile&& file, const File::Name& name) override;
    };
}