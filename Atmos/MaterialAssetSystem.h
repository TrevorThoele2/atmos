#pragma once

#include "AssetSystem.h"

#include "MaterialAsset.h"

namespace Atmos
{
    class MaterialAssetSystem : public AssetSystem<MaterialAsset>
    {
    public:
        MaterialAssetSystem(ObjectManager& manager);
        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(MaterialAssetSystem);
    private:
        void InitializeImpl() override;
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::MaterialAssetSystem)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };
}