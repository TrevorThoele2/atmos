#pragma once

#include "AudioAsset.h"
#include "SoundResource.h"

#include "Volume.h"
#include "Buffer.h"

#include "Logger.h"

namespace Atmos::Audio
{
    class Manager
    {
    public:
        Manager(const Manager& arg) = delete;
        Manager& operator=(const Manager& arg) = delete;
        virtual ~Manager() = 0;

        virtual std::unique_ptr<Asset::Resource::Audio> CreateAssetResource(const Buffer& buffer, const Name& name) = 0;
        virtual std::unique_ptr<Resource::Sound> CreateSoundResource(const Asset::Resource::Audio& asset, Volume volume) = 0;

        virtual void DestroyingSoundResource(Resource::Sound& resource) = 0;

        virtual void SetMasterVolume(Volume volume) = 0;

        virtual void PruneDoneResources() = 0;
        [[nodiscard]] virtual std::vector<Resource::Sound*> DoneResources() = 0;
    protected:
        Manager(Logging::Logger& logger, String typeName);
    private:
        String typeName;
        Logging::Logger* logger;
    };
}