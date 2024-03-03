#pragma once

#include <memory>

namespace Atmos::Asset
{
    class AudioAssetInstanceData;

    class AudioAssetData
    {
    public:
        virtual ~AudioAssetData() = 0;
        [[nodiscard]] virtual std::unique_ptr<AudioAssetData> Clone() const = 0;
        [[nodiscard]] virtual std::unique_ptr<AudioAssetInstanceData> CreateInstanceData() const = 0;
    };
}