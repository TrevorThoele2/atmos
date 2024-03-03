
#include "DXAudio.h"
#include "Environment.h"

namespace Atmos
{
    class AudioAssetInstanceData : public AudioAsset::Instance::Data
    {
    private:
        XAUDIO2_BUFFER buffer;
        IXAudio2SourceVoice *voice;
        const WAVEFORMATEX &format;
    public:
        AudioAssetInstanceData(const XAUDIO2_BUFFER &buffer, const WAVEFORMATEX &format) : buffer(buffer), voice(nullptr), format(format) {}
        AudioAssetInstanceData(const AudioAssetInstanceData &arg) : buffer(arg.buffer), voice(nullptr), format(arg.format)
        {}

        AudioAssetInstanceData* Clone() const override
        {
            return new AudioAssetInstanceData(*this);
        }

        void Start() override
        {
            voice->Start();
        }

        void Stop() override
        {
            voice->Stop();
        }

        void SetVolume(AudioAsset::Instance::Volume set) override
        {
            voice->SetVolume(set);
        }

        void Loop(bool set) override
        {
            (set) ? buffer.LoopCount = XAUDIO2_LOOP_INFINITE : buffer.LoopCount = 0;
            Resubmit();
        }

        bool Loop() const override
        {
            return buffer.LoopCount == XAUDIO2_LOOP_INFINITE;
        }

        bool IsPlaying() const override
        {
            XAUDIO2_VOICE_STATE state;
            voice->GetState(&state);
            return state.pCurrentBufferContext != nullptr;
        }

        void Resubmit() override
        {
            Environment::GetAudio<DX9AudioHandler>()->CreateSourceVoice(&voice, format);
            voice->SubmitSourceBuffer(&buffer);
        }
    };

    class AudioAssetData : public AudioAsset::Data
    {
    private:
        XAUDIO2_BUFFER buffer;
        WAVEFORMATEX format;
    public:
        AudioAssetData(XAUDIO2_BUFFER &&buffer, WAVEFORMATEX &&format) : buffer(std::move(buffer)), format(std::move(format))
        {}

        ~AudioAssetData()
        {
            delete[] buffer.pAudioData;
        }

        AudioAsset::Instance MakeInstance(const AudioAsset &asset) const override
        {
            return AudioAsset::Instance(asset, new AudioAssetInstanceData(buffer, format));
        }
    };

    AudioAsset DX9AudioHandler::CreateAudioImpl(Data &&data, const FileName &name)
    {
        BYTE *rawBuffer = nullptr;
        Data::first_type::SizeT rawBufferSize = 0;

        {
            auto &recovered = data.first.RecoverBytes<BYTE*>();
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
        format.nChannels = data.second.channels;
        format.nSamplesPerSec = data.second.samplesPerSec;
        format.nAvgBytesPerSec = data.second.avgBytesPerSec;
        format.cbSize = 0;
        format.nBlockAlign = data.second.blockAlign;
        format.wBitsPerSample = data.second.bitsPerSample;
        format.wFormatTag = data.second.formatTag;

        return AudioAsset(new AudioAssetData(std::move(buffer), std::move(format)), name);
    }

    void DX9AudioHandler::CreateMasteringVoice(IXAudio2MasteringVoice **voice)
    {
        pXAudio2->CreateMasteringVoice(voice);
    }

    DX9AudioHandler::DX9AudioHandler() : pXAudio2(nullptr), pMasterVoice(nullptr)
    {
        CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        // Fill the engine pointer
        XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
        CreateMasteringVoice(&pMasterVoice);
    }

    DX9AudioHandler::~DX9AudioHandler()
    {
        pXAudio2->Release();
    }

    void DX9AudioHandler::CreateSourceVoice(IXAudio2SourceVoice **voice, const WAVEFORMATEX &format)
    {
        pXAudio2->CreateSourceVoice(voice, &format);
    }

    bool DX9AudioHandler::SetMasterVolume(float setTo)
    {
        return SUCCEEDED(pMasterVoice->SetVolume(setTo));
    }
}