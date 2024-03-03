#pragma once

#include <memory>

namespace Atmos::Asset
{
    class ImageAssetData
    {
    public:
        virtual ~ImageAssetData() = 0;
        [[nodiscard]] virtual std::unique_ptr<ImageAssetData> Clone() const = 0;
    };
}