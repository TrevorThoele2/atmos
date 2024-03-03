#include "ShaderAssetFactory.h"

namespace Atmos
{
    ObjectFactory<Asset::ShaderAsset>::ObjectFactory(ObjectManager& manager) : FileAssetFactory(manager)
    {}
}