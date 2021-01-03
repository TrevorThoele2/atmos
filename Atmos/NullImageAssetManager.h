#pragma once

#include "ImageAssetManager.h"

namespace Atmos::Asset
{
    class NullImageManager final : public ImageManager
    {
    public:
        NullImageManager() = default;

        Resource::Loaded<Resource::Image> Load(const File::Path& filePath) override;
        Resource::Loaded<Resource::Image> Load(const Buffer& memory) override;
    };
}