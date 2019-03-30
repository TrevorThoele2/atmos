#pragma once

#include "AssetSystem.h"

#include "Material.h"

namespace Atmos
{
    class MaterialSystem : public AssetSystem<Material>
    {
    public:
        MaterialSystem(ObjectManager& manager);
        MaterialSystem(const ::Inscription::Table<MaterialSystem>& table);
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::MaterialSystem)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}