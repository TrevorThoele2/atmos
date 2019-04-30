
#include "ShaderAssetFactory.h"

namespace Atmos
{
    ObjectFactory<ShaderAsset>::ObjectFactory(ObjectManager& manager) : FileAssetFactory(manager)
    {}
}