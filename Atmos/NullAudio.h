#pragma once

#include "AudioHandlerBase.h"

namespace Atmos
{
    class NullAudioHandler : public AudioHandlerBase
    {
    private:
        NullAudioHandler(const NullAudioHandler &arg) = delete;
        NullAudioHandler& operator=(const NullAudioHandler &arg) = delete;

        AudioAsset CreateAudioImpl(Data &&data, const FileName &name) override;
    public:
        NullAudioHandler() = default;
        bool SetMasterVolume(float setTo) override;
    };
}