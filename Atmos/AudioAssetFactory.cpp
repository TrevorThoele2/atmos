
#include "AudioAssetFactory.h"

namespace Atmos
{
    ObjectFactory<Asset::AudioAsset>::ObjectFactory(ObjectManager& manager) : FileAssetFactory(manager)
    {}
}