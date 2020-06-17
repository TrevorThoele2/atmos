#pragma once

#include <memory>
#include "Volume.h"

namespace Atmos::Asset
{
    class AudioInstance;
}

namespace Atmos::Asset::Resource
{
    class AudioInstance
    {
    public:
        virtual ~AudioInstance() = 0;
    private:
        [[nodiscard]] virtual std::unique_ptr<AudioInstance> Clone() const = 0;
        virtual void Start() = 0;
        virtual void Stop() = 0;
        virtual void SetVolume(Atmos::Audio::Volume set) = 0;
        virtual void Loop(bool set) = 0;
        virtual void Resubmit() = 0;
    private:
        friend Atmos::Asset::AudioInstance;
    };
}