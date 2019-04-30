
#include "ImageAssetFactory.h"

namespace Atmos
{
    ObjectFactory<ImageAsset>::ObjectFactory(ObjectManager& manager) : FileAssetFactory(manager)
    {}
}