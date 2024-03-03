#include "ImageCore.h"

namespace Atmos::Render
{
    ImageCore::ImageCore(Arca::Index<Asset::Image> asset, Index assetIndex) : asset(asset), assetIndex(assetIndex)
    {}
}