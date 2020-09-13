#include "ImageCore.h"

namespace Atmos::Render
{
    ImageCore::ImageCore(
        Arca::Index<Asset::Image> asset,
        Index assetIndex,
        Arca::Index<Asset::Material> material,
        Color color)
        :
        asset(asset),
        assetIndex(assetIndex),
        material(material),
        color(color)
    {}
}