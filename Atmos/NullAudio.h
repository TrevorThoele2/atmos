#pragma once

#include "AudioManager.h"

namespace Atmos
{
    class NullAudioManager : public AudioManager
    {
    public:
        NullAudioManager() = default;
        bool SetMasterVolume(float setTo) override;
    private:
        NullAudioManager(const NullAudioManager& arg) = delete;
        NullAudioManager& operator=(const NullAudioManager& arg) = delete;

        std::unique_ptr<AudioAssetData> CreateAudioDataImpl(ExtractedFile&& file, const FileName& name) override;
    };
}