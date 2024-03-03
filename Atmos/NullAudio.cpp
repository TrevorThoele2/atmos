
#include "NullAudio.h"

#include "AudioAsset.h"
#include "AudioAssetInstance.h"

namespace Atmos
{
    class AudioAssetInstanceDataImplementation : public AudioAssetInstanceData
    {
    public:
        AudioAssetInstanceDataImplementation() = default;

        std::unique_ptr<AudioAssetInstanceData> Clone() const override
        {
            return std::unique_ptr<AudioAssetInstanceData>(new AudioAssetInstanceDataImplementation(*this));
        }

        void Start() override
        {}

        void Stop() override
        {}

        void SetVolume(Volume set) override
        {}

        void Loop(bool set) override
        {}

        void Resubmit() override
        {}
    };

    class AudioAssetDataImplementation : public AudioAssetData
    {
    public:
        AudioAssetDataImplementation() = default;

        std::unique_ptr<AudioAssetData> Clone() const override
        {
            return std::unique_ptr<AudioAssetData>(new AudioAssetDataImplementation(*this));
        }

        std::unique_ptr<AudioAssetInstanceData> CreateInstanceData() const override
        {
            return std::unique_ptr<AudioAssetInstanceData>(new AudioAssetInstanceDataImplementation());
        }
    };

    bool NullAudioHandler::SetMasterVolume(float setTo)
    {
        return true;
    }

    std::unique_ptr<AudioAssetData> NullAudioHandler::CreateAudioDataImpl(ExtractedFile&& file, const FileName& name)
    {
        return std::make_unique<AudioAssetDataImplementation>();
    }
}