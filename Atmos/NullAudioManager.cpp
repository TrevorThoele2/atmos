#include "NullAudioManager.h"

#include "AudioAsset.h"
#include "AudioAssetInstance.h"

namespace Atmos::Audio
{
    class AudioAssetInstanceDataImplementation final : public Asset::AudioAssetInstanceData
    {
    public:
        AudioAssetInstanceDataImplementation() = default;

        [[nodiscard]] std::unique_ptr<AudioAssetInstanceData> Clone() const override
        {
            return std::make_unique<AudioAssetInstanceDataImplementation>(*this);
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

    class AudioAssetDataImplementation final : public Asset::AudioAssetData
    {
    public:
        AudioAssetDataImplementation() = default;

        [[nodiscard]] std::unique_ptr<AudioAssetData> Clone() const override
        {
            return std::make_unique<AudioAssetDataImplementation>(*this);
        }

        [[nodiscard]] std::unique_ptr<Asset::AudioAssetInstanceData> CreateInstanceData() const override
        {
            return std::make_unique<AudioAssetInstanceDataImplementation>();
        }
    };

    bool NullAudioManager::SetMasterVolume(float set)
    {
        return true;
    }

    std::unique_ptr<Asset::AudioAssetData> NullAudioManager::CreateAudioDataImpl(
        ExtractedFile&& file, const File::Name& name)
    {
        return std::make_unique<AudioAssetDataImplementation>();
    }
}