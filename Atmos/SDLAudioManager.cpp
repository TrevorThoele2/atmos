#include "SDLAudioManager.h"

#include "SDLAudioAssetResource.h"
#include "SDLSoundResource.h"
#include "AudioError.h"
#include "SDLIncludes.h"

#include "StringUtility.h"

namespace Atmos::Audio::SDL
{
    Manager::Manager(Logging::Logger& logger) : Audio::Manager(logger, "SDL")
    {
        const auto initFlags = MIX_INIT_OGG;
        const auto initResult = Mix_Init(initFlags);
        if (initResult != initFlags)
            throw Error("Could not initialize all audio file type handling.", { {"Reason", Mix_GetError()} });

        const auto deviceID = Mix_OpenAudioDevice(
            MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096, nullptr, SDL_AUDIO_ALLOW_ANY_CHANGE);
        if (deviceID < 0)
            throw Error("Could not open audio device.", { {"Reason", Mix_GetError()} });

        const Channel maxChannels = 1024;
        const auto allocatedChannels = Mix_AllocateChannels(maxChannels - 1);
        if (allocatedChannels != maxChannels - 1)
            logger.Log(
                "Could not allocate all audio channels.",
                Logging::Severity::Warning,
                Logging::Details
                {
                    {"Requested", ToString(maxChannels - 1)},
                    {"Allocated", ToString(allocatedChannels)}
                });

        const auto groupedChannels = Mix_GroupChannels(0, maxChannels, group);
        if (groupedChannels != maxChannels)
            logger.Log(
                "Could not group all audio channels.",
                Logging::Severity::Warning,
                Logging::Details
                {
                    {"Requested", ToString(maxChannels)},
                    {"Allocated", ToString(groupedChannels)}
                });

        self = this;
        Mix_ChannelFinished(&OnChannelDone);
    }

    Manager::~Manager()
    {
        Mix_CloseAudio();
    }

    std::unique_ptr<Asset::Resource::Audio> Manager::CreateAssetResource(const Buffer& buffer, const Name& name)
    {
        return std::make_unique<Asset::Resource::SDL::Audio>(buffer);
    }

    std::unique_ptr<Resource::Sound> Manager::CreateSoundResource(const Asset::Resource::Audio& asset, Volume volume)
    {
        const auto sdlAsset = dynamic_cast<const Asset::Resource::SDL::Audio*>(&asset);
        if (!sdlAsset)
            throw Error("Audio asset resource was not the correct type.");

        const auto ops = sdlAsset->ops;
        const auto channel = NextChannel();

        auto resource = std::make_unique<Resource::SDL::Sound>(ops, channel, volume);
        channelToResources.emplace(channel, resource.get());
        return resource;
    }

    void Manager::DestroyingSoundResource(Resource::Sound& resource)
    {
        for(auto channelToResource = channelToResources.begin(); channelToResource != channelToResources.end(); ++channelToResource)
        {
            if (channelToResource->second == &resource)
            {
                channelToResources.erase(channelToResource);
                return;
            }
        }
    }

    void Manager::SetMasterVolume(float volume)
    {
        
    }

    void Manager::PruneDoneResources()
    {
        doneResources = {};
    }

    std::vector<Resource::Sound*> Manager::DoneResources()
    {
        return doneResources;
    }

    Manager* Manager::self = nullptr;

    int Manager::NextChannel() const
    {
        const auto nextChannel = Mix_GroupAvailable(group);
        return nextChannel != -1 ? nextChannel : 0;
    }

    void Manager::OnChannelDone(Channel channel)
    {
        const auto channelToResource = self->channelToResources.find(channel);
        if (channelToResource == self->channelToResources.end())
            return;

        self->doneResources.push_back(channelToResource->second);
    }
}