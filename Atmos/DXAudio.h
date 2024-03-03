#pragma once

#include <XAudio2.h>
#pragma comment (lib, "dsound.lib")

#include "AudioManager.h"

namespace Atmos
{
    class DX9AudioManager : public AudioManager
    {
    public:
        DX9AudioManager();
        ~DX9AudioManager();
        void CreateSourceVoice(IXAudio2SourceVoice** voice, const WAVEFORMATEX& format);
        bool SetMasterVolume(float setTo) override;
    private:
        IXAudio2* pXAudio2;
        IXAudio2MasteringVoice* pMasterVoice;

        DX9AudioManager(const DX9AudioManager& arg) = delete;
        DX9AudioManager& operator=(const DX9AudioManager& arg) = delete;

        std::unique_ptr<AudioAssetData> CreateAudioDataImpl(ExtractedFile&& file, const FileName& name) override;
        void CreateMasteringVoice(IXAudio2MasteringVoice** voice);
    };
}