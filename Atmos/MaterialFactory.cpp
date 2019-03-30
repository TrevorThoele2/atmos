
#include "MaterialFactory.h"

namespace Atmos
{
    ObjectFactory<Material>::ObjectFactory(ObjectManager& manager) : AssetFactory(manager)
    {}
}