
#include "AudioAssetFactory.h"

namespace Atmos
{
    ObjectFactory<AudioAsset>::ObjectFactory(ObjectManager& manager) : FileAssetFactory(manager)
    {}
}