#include "NullAudioManager.h"

#include "AudioAssetResource.h"
#include "SoundResource.h"

namespace Atmos::Audio
{
    class AssetResource final : public Asset::Resource::Audio
    {
    public:
        AssetResource() = default;
    };

    class SoundResource final : public Resource::Sound
    {
    public:
        SoundResource() = default;

        void Restart() override
        {}

        void Pause() override
        {}

        void Resume() override
        {}

        void SetVolume(Volume volume) override
        {}

        void SetPosition(const Spatial::Point2D & position) override
        {}
    };


    std::unique_ptr<Asset::Resource::Audio> NullManager::CreateAssetResource(const Buffer& buffer, const Name& name)
    {
        return std::make_unique<AssetResource>();
    }

    std::unique_ptr<Resource::Sound> NullManager::CreateSoundResource(const Asset::Resource::Audio& asset, Volume volume)
    {
        return std::make_unique<SoundResource>();
    }

    void NullManager::DestroyingSoundResource(Resource::Sound& resource)
    {}

    void NullManager::SetMasterVolume(Volume volume)
    {}

    void NullManager::PruneDoneResources()
    {}

    std::vector<Resource::Sound*> NullManager::DoneResources()
    {
        return {};
    }

    String NullManager::TypeName() const
    {
        return "Null";
    }
}