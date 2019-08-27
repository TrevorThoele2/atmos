#include "DirectX9AudioManager.h"

#include "AudioAsset.h"
#include "AudioAssetInstance.h"

namespace Atmos::Audio
{
    class AudioAssetInstanceDataImplementation final : public Asset::AudioAssetInstanceData
    {
    public:
        AudioAssetInstanceDataImplementation(
            DirectX9AudioManager& owner, const XAUDIO2_BUFFER& buffer, const WAVEFORMATEX& format) :
            owner(&owner), buffer(buffer), sourceVoice(nullptr), waveFormat(format)
        {}

        AudioAssetInstanceDataImplementation(const AudioAssetInstanceDataImplementation& arg) :
            owner(arg.owner), buffer(arg.buffer), sourceVoice(nullptr), waveFormat(arg.waveFormat)
        {}

        ~AudioAssetInstanceDataImplementation()
        {
            AudioAssetInstanceDataImplementation::Stop();
        }

        [[nodiscard]] std::unique_ptr<AudioAssetInstanceData> Clone() const override
        {
            return std::make_unique<AudioAssetInstanceDataImplementation>(*this);
        }

        void Start() override
        {
            sourceVoice->Start();
        }

        void Stop() override
        {
            sourceVoice->Stop();
        }

        void SetVolume(Volume set) override
        {
            sourceVoice->SetVolume(set);
        }

        void Loop(bool set) override
        {
            (set) ? buffer.LoopCount = XAUDIO2_LOOP_INFINITE : buffer.LoopCount = 0;
            Resubmit();
        }

        void Resubmit() override
        {
            owner->CreateSourceVoice(&sourceVoice, waveFormat);
            sourceVoice->SubmitSourceBuffer(&buffer);
        }
    private:
        DirectX9AudioManager* owner;
    private:
        XAUDIO2_BUFFER buffer;
        IXAudio2SourceVoice* sourceVoice;
        const WAVEFORMATEX& waveFormat;
    };

    class AudioAssetDataImplementation final : public Asset::AudioAssetData
    {
    public:
        AudioAssetDataImplementation(DirectX9AudioManager& owner, XAUDIO2_BUFFER&& buffer, WAVEFORMATEX&& format)
            : owner(&owner), buffer(buffer), waveFormat(format)
        {}

        ~AudioAssetDataImplementation()
        {
            delete[] buffer.pAudioData;
        }

        [[nodiscard]] std::unique_ptr<AudioAssetData> Clone() const override
        {
            return std::unique_ptr<AudioAssetData>(std::make_unique<AudioAssetDataImplementation>(*this));
        }

        [[nodiscard]] std::unique_ptr<Asset::AudioAssetInstanceData> CreateInstanceData() const override
        {
            return std::make_unique<AudioAssetInstanceDataImplementation>(*owner, buffer, waveFormat);
        }
    private:
        DirectX9AudioManager* owner;
    private:
        XAUDIO2_BUFFER buffer;
        WAVEFORMATEX waveFormat;
    };

    DirectX9AudioManager::DirectX9AudioManager()
    {
        CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        // Fill the engine pointer
        XAudio2Create(&xaudio2Interface, 0, XAUDIO2_DEFAULT_PROCESSOR);
        CreateMasteringVoice(&masteringVoice);
    }

    DirectX9AudioManager::~DirectX9AudioManager()
    {
        xaudio2Interface->Release();
    }

    void DirectX9AudioManager::CreateSourceVoice(IXAudio2SourceVoice** sourceVoice, const WAVEFORMATEX& waveFormat)
    {
        xaudio2Interface->CreateSourceVoice(sourceVoice, &waveFormat);
    }

    bool DirectX9AudioManager::SetMasterVolume(float set)
    {
        return SUCCEEDED(masteringVoice->SetVolume(set));
    }

    std::unique_ptr<Asset::AudioAssetData> DirectX9AudioManager::CreateAudioDataImpl(
        ExtractedFile&& file, const File::Name& name)
    {
        const auto recovered = file.first.RecoverBytes<BYTE*>();
        const auto rawBuffer = recovered.first;
        const auto rawBufferSize = recovered.second;

        XAUDIO2_BUFFER buffer;
        buffer.AudioBytes = rawBufferSize;
        buffer.Flags = 0;
        buffer.LoopBegin = 0;
        buffer.LoopCount = 0;
        buffer.LoopLength = 0;
        buffer.pAudioData = rawBuffer;
        buffer.pContext = nullptr;
        buffer.PlayBegin = 0;
        buffer.PlayLength = 0;

        WAVEFORMATEX waveFormat;
        waveFormat.nChannels = file.second.channels;
        waveFormat.nSamplesPerSec = file.second.samplesPerSec;
        waveFormat.nAvgBytesPerSec = file.second.avgBytesPerSec;
        waveFormat.cbSize = 0;
        waveFormat.nBlockAlign = file.second.blockAlign;
        waveFormat.wBitsPerSample = file.second.bitsPerSample;
        waveFormat.wFormatTag = file.second.formatTag;

        return std::make_unique<AudioAssetDataImplementation>(*this, std::move(buffer), std::move(waveFormat));
    }

    void DirectX9AudioManager::CreateMasteringVoice(IXAudio2MasteringVoice** sourceVoice)
    {
        xaudio2Interface->CreateMasteringVoice(sourceVoice);
    }
}