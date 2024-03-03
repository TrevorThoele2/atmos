
#include "MaterialAssetFactory.h"

namespace Atmos
{
    ObjectFactory<Asset::MaterialAsset>::ObjectFactory(ObjectManager& manager) : AssetFactory(manager)
    {}
}