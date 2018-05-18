
#include "NullAudio.h"

namespace Atmos
{
    class AudioAssetInstanceData : public AudioAsset::Instance::Data
    {
    public:
        AudioAssetInstanceData() = default;

        AudioAssetInstanceData* Clone() const override
        {
            return new AudioAssetInstanceData(*this);
        }

        void Start() override
        {}

        void Stop() override
        {}

        void SetVolume(AudioAsset::Instance::Volume set) override
        {}

        void Loop(bool set) override
        {}

        bool Loop() const override
        {
            return false;
        }

        bool IsPlaying() const override
        {
            return false;
        }

        void Resubmit() override
        {}
    };

    class AudioAssetData : public AudioAsset::Data
    {
    public:
        AudioAssetData() = default;

        AudioAsset::Instance MakeInstance(const AudioAsset &res) const override
        {
            return AudioAsset::Instance(res, new AudioAssetInstanceData());
        }
    };

    AudioAsset NullAudioHandler::CreateAudioImpl(Data &&data, const FileName &name)
    {
        return AudioAsset(new AudioAssetData(), name);
    }

    bool NullAudioHandler::SetMasterVolume(float setTo)
    {
        return true;
    }
}