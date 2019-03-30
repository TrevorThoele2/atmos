
#include "AudioFactory.h"

namespace Atmos
{
    ObjectFactory<AudioAsset>::ObjectFactory(ObjectManager& manager) : FileAssetFactory(manager)
    {}
}