
#include "ScriptAssetFactory.h"

namespace Atmos
{
    ObjectFactory<ScriptAsset>::ObjectFactory(ObjectManager& manager) : FileAssetFactory(manager)
    {}
}