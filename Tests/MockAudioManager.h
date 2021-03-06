#pragma once

#include <Atmos/AudioManager.h>

class MockAudioManager final : public Atmos::Audio::Manager
{
public:
    std::vector<Atmos::Audio::Resource::Sound*> doneResources;
public:
    MockAudioManager(Atmos::Logging::Logger& logger);

    std::unique_ptr<Atmos::Asset::Resource::Audio> CreateAssetResource(const Atmos::Buffer& buffer, const Atmos::Name& name) override;
    std::unique_ptr<Atmos::Audio::Resource::Sound> CreateSoundResource(const Atmos::Asset::Resource::Audio& asset, Atmos::Audio::Volume volume) override;

    void DestroyingSoundResource(Atmos::Audio::Resource::Sound& resource) override;

    void SetMasterVolume(Atmos::Audio::Volume volume) override;

    void PruneDoneResources() override;
    [[nodiscard]] std::vector<Atmos::Audio::Resource::Sound*> DoneResources() override;
};