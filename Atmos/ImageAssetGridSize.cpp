#include "ImageAssetGridSize.h"

namespace Atmos::Asset
{
    bool ImageGridSize::operator==(const ImageGridSize& arg) const
    {
        return columns == arg.columns && rows == arg.rows;
    }

    bool ImageGridSize::operator!=(const ImageGridSize& arg) const
    {
        return !(*this == arg);
    }
}