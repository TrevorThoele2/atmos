#include "ImageAssetFactory.h"

namespace Atmos
{
    ObjectFactory<Asset::ImageAsset>::ObjectFactory(ObjectManager& manager) : FileAssetFactory(manager)
    {}
}