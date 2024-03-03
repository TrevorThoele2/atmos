#pragma once

#include "AudioManager.h"

#include "SDLChannel.h"

#include "Logger.h"

namespace Atmos::Audio::SDL
{
    class Manager final : public Audio::Manager
    {
    public:
        Manager(Logging::Logger& logger);
        Manager(const Manager& arg) = delete;
        ~Manager();

        Manager& operator=(const Manager& arg) = delete;

        std::unique_ptr<Asset::Resource::Audio> CreateAssetResource(const Buffer& buffer, const Name& name) override;
        std::unique_ptr<Resource::Sound> CreateSoundResource(const Asset::Resource::Audio& asset, Volume volume) override;

        void DestroyingSoundResource(Resource::Sound& resource) override;

        void SetMasterVolume(float volume) override;

        void PruneDoneResources() override;
        [[nodiscard]] std::vector<Resource::Sound*> DoneResources() override;

        [[nodiscard]] String TypeName() const override;
    private:
        std::unordered_map<Channel, Resource::Sound*> channelToResources;
        std::vector<Resource::Sound*> doneResources;

        using Group = int;
        const Group group = 0;

        static Manager* self;

        [[nodiscard]] Channel NextChannel() const;
        static void OnChannelDone(Channel channel);
    private:
        Logging::Logger* logger;
    };
}