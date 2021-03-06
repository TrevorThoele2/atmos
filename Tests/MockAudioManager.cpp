#include "MockAudioManager.h"

#include "MockSoundResource.h"

MockAudioManager::MockAudioManager(Atmos::Logging::Logger& logger) :
    Manager(logger, "Mock")
{}

std::unique_ptr<Atmos::Asset::Resource::Audio> MockAudioManager::CreateAssetResource(const Atmos::Buffer& buffer, const Atmos::Name& name)
{
    return {};
}

std::unique_ptr<Atmos::Audio::Resource::Sound> MockAudioManager::CreateSoundResource(const Atmos::Asset::Resource::Audio& asset, Audio::Volume volume)
{
    return std::make_unique<MockSoundResource>();
}

void MockAudioManager::DestroyingSoundResource(Atmos::Audio::Resource::Sound& resource)
{}

void MockAudioManager::SetMasterVolume(Atmos::Audio::Volume volume)
{}

void MockAudioManager::PruneDoneResources()
{}

std::vector<Atmos::Audio::Resource::Sound*> MockAudioManager::DoneResources()
{
    return doneResources;
}