#pragma once

#include "AudioHandlerBase.h"

namespace Atmos
{
    class NullAudioHandler : public AudioHandlerBase
    {
    public:
        NullAudioHandler() = default;
        bool SetMasterVolume(float setTo) override;
    private:
        NullAudioHandler(const NullAudioHandler& arg) = delete;
        NullAudioHandler& operator=(const NullAudioHandler& arg) = delete;

        std::unique_ptr<AudioAssetData> CreateAudioDataImpl(ExtractedFile&& file, const FileName& name) override;
    };
}