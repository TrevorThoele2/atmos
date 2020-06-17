#include "DirectX9AudioManager.h"

#include "AudioAsset.h"
#include "AudioAssetInstance.h"
#include "AudioAssetInstanceResource.h"

namespace Atmos::Audio
{
    class AudioAssetInstanceResourceImplementation final : public Asset::Resource::AudioInstance
    {
    public:
        AudioAssetInstanceResourceImplementation(
            DirectX9AudioManager& owner, const XAUDIO2_BUFFER& buffer, const WAVEFORMATEX& format) :
            owner(&owner), buffer(buffer), sourceVoice(nullptr), waveFormat(format)
        {}

        AudioAssetInstanceResourceImplementation(const AudioAssetInstanceResourceImplementation& arg) :
            owner(arg.owner), buffer(arg.buffer), sourceVoice(nullptr), waveFormat(arg.waveFormat)
        {}

        ~AudioAssetInstanceResourceImplementation()
        {
            AudioAssetInstanceResourceImplementation::Stop();
        }

        [[nodiscard]] std::unique_ptr<AudioInstance> Clone() const override
        {
            return std::make_unique<AudioAssetInstanceResourceImplementation>(*this);
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
            buffer.LoopCount = set
                ? XAUDIO2_LOOP_INFINITE
                : 0;
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

    class AudioAssetDataImplementation final : public Asset::Resource::Audio
    {
    public:
        AudioAssetDataImplementation(
            DirectX9AudioManager& owner,
            std::vector<unsigned char>&& audioData,
            const XAUDIO2_BUFFER& buffer,
            const WAVEFORMATEX& format)
            :
            owner(&owner),
            audioData(std::move(audioData)),
            buffer(buffer),
            waveFormat(format)
        {}

        [[nodiscard]] std::unique_ptr<Asset::Resource::AudioInstance> CreateInstanceResource() const override
        {
            return std::make_unique<AudioAssetInstanceResourceImplementation>(*owner, buffer, waveFormat);
        }
    private:
        DirectX9AudioManager* owner;
    private:
        std::vector<unsigned char> audioData;
        XAUDIO2_BUFFER buffer;
        WAVEFORMATEX waveFormat;
    };

    DirectX9AudioManager::DirectX9AudioManager()
    {
        CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        // Fill the engine pointer
        XAudio2Create(&xAudio2Interface, 0, XAUDIO2_DEFAULT_PROCESSOR);
        CreateMasteringVoice(&masteringVoice);
    }

    DirectX9AudioManager::~DirectX9AudioManager()
    {
        xAudio2Interface->Release();
    }

    void DirectX9AudioManager::CreateSourceVoice(IXAudio2SourceVoice** sourceVoice, const WAVEFORMATEX& waveFormat)
    {
        xAudio2Interface->CreateSourceVoice(sourceVoice, &waveFormat);
    }

    bool DirectX9AudioManager::SetMasterVolume(Volume set)
    {
        return SUCCEEDED(masteringVoice->SetVolume(set));
    }

    std::unique_ptr<Asset::Resource::Audio> DirectX9AudioManager::CreateAudioResourceImpl(
        const FormattedBuffer& file, const Name& name)
    {
        auto audioData = std::vector<unsigned char>(file.buffer.size());
        CopyBuffer(file.buffer.data(), audioData.data(), file.buffer.size(), 0);

        XAUDIO2_BUFFER buffer;
        buffer.AudioBytes = file.buffer.size();
        buffer.Flags = 0;
        buffer.LoopBegin = 0;
        buffer.LoopCount = 0;
        buffer.LoopLength = 0;
        buffer.pAudioData = audioData.data();
        buffer.pContext = nullptr;
        buffer.PlayBegin = 0;
        buffer.PlayLength = 0;

        WAVEFORMATEX waveFormat;
        waveFormat.nChannels = file.format.channels;
        waveFormat.nSamplesPerSec = file.format.samplesPerSec;
        waveFormat.nAvgBytesPerSec = file.format.avgBytesPerSec;
        waveFormat.cbSize = 0;
        waveFormat.nBlockAlign = file.format.blockAlign;
        waveFormat.wBitsPerSample = file.format.bitsPerSample;
        waveFormat.wFormatTag = file.format.formatTag;

        return std::make_unique<AudioAssetDataImplementation>(
            *this,
            std::move(audioData),
            buffer,
            waveFormat);
    }

    void DirectX9AudioManager::CreateMasteringVoice(IXAudio2MasteringVoice** sourceVoice)
    {
        xAudio2Interface->CreateMasteringVoice(sourceVoice);
    }
}