#include "NullAudioManager.h"

#include "AudioAsset.h"
#include "AudioAssetInstance.h"
#include "AudioAssetInstanceResource.h"

namespace Atmos::Audio
{
    class AudioAssetInstanceDataImplementation final : public Asset::Resource::AudioInstance
    {
    public:
        AudioAssetInstanceDataImplementation() = default;

        [[nodiscard]] std::unique_ptr<AudioInstance> Clone() const override
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

    class AudioAssetDataImplementation final : public Asset::Resource::Audio
    {
    public:
        AudioAssetDataImplementation() = default;

        [[nodiscard]] std::unique_ptr<Asset::Resource::AudioInstance> CreateInstanceResource() const override
        {
            return std::make_unique<AudioAssetInstanceDataImplementation>();
        }
    };

    bool NullAudioManager::SetMasterVolume(float set)
    {
        return true;
    }

    std::unique_ptr<Asset::Resource::Audio> NullAudioManager::CreateAudioResourceImpl(
        const FormattedBuffer& buffer, const Name& name)
    {
        return std::make_unique<AudioAssetDataImplementation>();
    }
}