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

namespace Inscription
{
    void Scribe<Atmos::Render::ImageCore, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        archive(object.asset);
        archive(object.assetIndex);
        archive(object.material);
        archive(object.color);
    }
}