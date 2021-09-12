#include "ImageCore.h"

namespace Atmos::Render
{
    ImageCore::ImageCore(Arca::Index<Asset::Image> asset, Index assetIndex) : asset(asset), assetIndex(assetIndex)
    {}

    bool ImageCore::operator==(const ImageCore& arg) const
    {
        return IndexEqualsValue(asset, arg.asset) && assetIndex == arg.assetIndex;
    }

    bool ImageCore::operator!=(const ImageCore& arg) const
    {
        return !(*this == arg);
    }
}