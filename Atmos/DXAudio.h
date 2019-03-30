#pragma once

#include <XAudio2.h>
#pragma comment (lib, "dsound.lib")

#include "AudioHandlerBase.h"

namespace Atmos
{
    class DX9AudioHandler : public AudioHandlerBase
    {
    public:
        DX9AudioHandler();
        ~DX9AudioHandler();
        void CreateSourceVoice(IXAudio2SourceVoice** voice, const WAVEFORMATEX& format);
        bool SetMasterVolume(float setTo) override;
    private:
        IXAudio2* pXAudio2;
        IXAudio2MasteringVoice* pMasterVoice;

        DX9AudioHandler(const DX9AudioHandler& arg) = delete;
        DX9AudioHandler& operator=(const DX9AudioHandler& arg) = delete;

        std::unique_ptr<AudioAssetData> CreateAudioDataImpl(ExtractedFile&& file, const FileName& name) override;
        void CreateMasteringVoice(IXAudio2MasteringVoice** voice);
    };
}