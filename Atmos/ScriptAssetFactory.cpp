
#include "ScriptAssetFactory.h"

namespace Atmos
{
    ObjectFactory<Asset::ScriptAsset>::ObjectFactory(ObjectManager& manager) : FileAssetFactory(manager)
    {}
}