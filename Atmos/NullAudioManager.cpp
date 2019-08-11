#include "NullAudioManager.h"

#include "AudioAsset.h"
#include "AudioAssetInstance.h"

namespace Atmos::Audio
{
    class AudioAssetInstanceDataImplementation : public Asset::AudioAssetInstanceData
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

    class AudioAssetDataImplementation : public Asset::AudioAssetData
    {
    public:
        AudioAssetDataImplementation() = default;

        std::unique_ptr<AudioAssetData> Clone() const override
        {
            return std::unique_ptr<AudioAssetData>(new AudioAssetDataImplementation(*this));
        }

        std::unique_ptr<Asset::AudioAssetInstanceData> CreateInstanceData() const override
        {
            return std::unique_ptr<Asset::AudioAssetInstanceData>(new AudioAssetInstanceDataImplementation());
        }
    };

    bool NullAudioManager::SetMasterVolume(float setTo)
    {
        return true;
    }

    std::unique_ptr<Asset::AudioAssetData> NullAudioManager::CreateAudioDataImpl(
        ExtractedFile&& file, const File::Name& name)
    {
        return std::make_unique<AudioAssetDataImplementation>();
    }
}