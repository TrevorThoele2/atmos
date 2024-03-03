#pragma once

#include <XAudio2.h>
#pragma comment (lib, "dsound.lib")

#include "AudioManager.h"

namespace Atmos::Audio
{
    class DirectX9AudioManager final : public AudioManager
    {
    public:
        DirectX9AudioManager();
        DirectX9AudioManager(const DirectX9AudioManager& arg) = delete;
        DirectX9AudioManager& operator=(const DirectX9AudioManager& arg) = delete;
        ~DirectX9AudioManager();

        void CreateSourceVoice(IXAudio2SourceVoice** sourceVoice, const WAVEFORMATEX& waveFormat);
        bool SetMasterVolume(float set) override;
    private:
        IXAudio2* xaudio2Interface = nullptr;
        IXAudio2MasteringVoice* masteringVoice = nullptr;
    private:
        std::unique_ptr<Asset::AudioAssetData> CreateAudioDataImpl(
            ExtractedFile&& file,
            const File::Name& name) override;
        void CreateMasteringVoice(IXAudio2MasteringVoice** sourceVoice);
    };
}