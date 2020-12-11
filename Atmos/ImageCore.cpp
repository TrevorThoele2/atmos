#include "ImageCore.h"

namespace Atmos::Render
{
    ImageCore::ImageCore(
        Arca::Index<Asset::Image> asset,
        Index assetIndex,
        Arca::Index<Asset::ImageMaterial> material,
        Color color)
        :
        asset(asset),
        assetIndex(assetIndex),
        material(material),
        color(color)
    {}
}