
#include "DXAudio.h"

#include "AudioAsset.h"
#include "AudioAssetInstance.h"

namespace Atmos
{
    class AudioAssetInstanceDataImplementation : public AudioAssetInstanceData
    {
    public:
        AudioAssetInstanceDataImplementation(DX9AudioManager& owner, const XAUDIO2_BUFFER& buffer, const WAVEFORMATEX& format) :
            owner(&owner), buffer(buffer), voice(nullptr), format(format)
        {}

        AudioAssetInstanceDataImplementation(const AudioAssetInstanceDataImplementation& arg) :
            owner(arg.owner), buffer(arg.buffer), voice(nullptr), format(arg.format)
        {}

        ~AudioAssetInstanceDataImplementation()
        {
            Stop();
        }

        std::unique_ptr<AudioAssetInstanceData> Clone() const override
        {
            return std::unique_ptr<AudioAssetInstanceData>(new AudioAssetInstanceDataImplementation(*this));
        }

        void Start() override
        {
            voice->Start();
        }

        void Stop() override
        {
            voice->Stop();
        }

        void SetVolume(Volume set) override
        {
            voice->SetVolume(set);
        }

        void Loop(bool set) override
        {
            (set) ? buffer.LoopCount = XAUDIO2_LOOP_INFINITE : buffer.LoopCount = 0;
            Resubmit();
        }

        void Resubmit() override
        {
            owner->CreateSourceVoice(&voice, format);
            voice->SubmitSourceBuffer(&buffer);
        }
    private:
        DX9AudioManager* owner;
    private:
        XAUDIO2_BUFFER buffer;
        IXAudio2SourceVoice* voice;
        const WAVEFORMATEX& format;
    };

    class AudioAssetDataImplementation : public AudioAssetData
    {
    public:
        AudioAssetDataImplementation(DX9AudioManager& owner, XAUDIO2_BUFFER&& buffer, WAVEFORMATEX&& format) :
            owner(&owner), buffer(std::move(buffer)), format(std::move(format))
        {}

        ~AudioAssetDataImplementation()
        {
            delete[] buffer.pAudioData;
        }

        std::unique_ptr<AudioAssetData> Clone() const override
        {
            return std::unique_ptr<AudioAssetData>(new AudioAssetDataImplementation(*this));
        }

        std::unique_ptr<AudioAssetInstanceData> CreateInstanceData() const override
        {
            return std::unique_ptr<AudioAssetInstanceData>(new AudioAssetInstanceDataImplementation(*owner, buffer, format));
        }
    private:
        DX9AudioManager* owner;
    private:
        XAUDIO2_BUFFER buffer;
        WAVEFORMATEX format;
    };

    DX9AudioManager::DX9AudioManager() : pXAudio2(nullptr), pMasterVoice(nullptr)
    {
        CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        // Fill the engine pointer
        XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
        CreateMasteringVoice(&pMasterVoice);
    }

    DX9AudioManager::~DX9AudioManager()
    {
        pXAudio2->Release();
    }

    void DX9AudioManager::CreateSourceVoice(IXAudio2SourceVoice** voice, const WAVEFORMATEX& format)
    {
        pXAudio2->CreateSourceVoice(voice, &format);
    }

    bool DX9AudioManager::SetMasterVolume(float setTo)
    {
        return SUCCEEDED(pMasterVoice->SetVolume(setTo));
    }

    std::unique_ptr<AudioAssetData> DX9AudioManager::CreateAudioDataImpl(ExtractedFile&& file, const FileName& name)
    {
        BYTE *rawBuffer = nullptr;
        ExtractedFile::first_type::SizeT rawBufferSize = 0;

        {
            auto &recovered = file.first.RecoverBytes<BYTE*>();
            rawBuffer = recovered.first;
            rawBufferSize = recovered.second;
        }

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

        WAVEFORMATEX format;
        format.nChannels = file.second.channels;
        format.nSamplesPerSec = file.second.samplesPerSec;
        format.nAvgBytesPerSec = file.second.avgBytesPerSec;
        format.cbSize = 0;
        format.nBlockAlign = file.second.blockAlign;
        format.wBitsPerSample = file.second.bitsPerSample;
        format.wFormatTag = file.second.formatTag;

        return std::make_unique<AudioAssetDataImplementation>(*this, std::move(buffer), std::move(format));
    }

    void DX9AudioManager::CreateMasteringVoice(IXAudio2MasteringVoice** voice)
    {
        pXAudio2->CreateMasteringVoice(voice);
    }
}