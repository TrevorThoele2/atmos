#pragma once

#include <XAudio2.h>
#pragma comment (lib, "dsound.lib")

#include "AudioManager.h"

namespace Atmos::Audio
{
    class DirectX9AudioManager : public AudioManager
    {
    public:
        DirectX9AudioManager();
        ~DirectX9AudioManager();

        void CreateSourceVoice(IXAudio2SourceVoice** voice, const WAVEFORMATEX& format);
        bool SetMasterVolume(float setTo) override;
    private:
        IXAudio2* pXAudio2;
        IXAudio2MasteringVoice* pMasterVoice;

        DirectX9AudioManager(const DirectX9AudioManager& arg) = delete;
        DirectX9AudioManager& operator=(const DirectX9AudioManager& arg) = delete;

        std::unique_ptr<Asset::AudioAssetData> CreateAudioDataImpl(ExtractedFile&& file, const File::Name& name) override;
        void CreateMasteringVoice(IXAudio2MasteringVoice** voice);
    };
}