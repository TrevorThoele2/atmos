#include "ImageAssetData.h"

namespace Atmos::Asset
{
    ImageAssetData::~ImageAssetData() = default;

    auto ImageAssetData::Width() const -> Dimension
    {
        return width;
    }

    auto ImageAssetData::Height() const -> Dimension
    {
        return height;
    }

    ImageAssetData::ImageAssetData(Dimension width, Dimension height) : width(width), height(height)
    {}
}