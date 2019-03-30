#pragma once

#include "AssetFactory.h"

#include "Material.h"

namespace Atmos
{
    template<>
    class ObjectFactory<Material> : public AssetFactory<Material>
    {
    public:
        ObjectFactory(ObjectManager& manager);
    };
}