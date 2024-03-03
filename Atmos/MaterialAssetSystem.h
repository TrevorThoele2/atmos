#pragma once

#include "AssetSystem.h"

#include "MaterialAsset.h"

namespace Atmos
{
    class MaterialAssetSystem : public AssetSystem<MaterialAsset>
    {
    public:
        MaterialAssetSystem(ObjectManager& manager);
        MaterialAssetSystem(const ::Inscription::Table<MaterialAssetSystem>& table);
    private:
        void InitializeImpl() override;
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::MaterialAssetSystem)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}