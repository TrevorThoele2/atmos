#pragma once

#include <memory>

namespace Atmos::Asset::Resource
{
    class AudioInstance;

    class Audio
    {
    public:
        virtual ~Audio() = 0;
        [[nodiscard]] virtual std::unique_ptr<AudioInstance> CreateInstanceResource() const = 0;
    protected:
        Audio() = default;
    };
}