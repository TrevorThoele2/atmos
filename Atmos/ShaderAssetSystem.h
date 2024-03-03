#pragma once

#include "AssetSystem.h"

#include "ShaderAsset.h"

#include "Serialization.h"

namespace Atmos
{
    class ShaderAssetSystem : public AssetSystem<ShaderAsset>
    {
    public:
        ShaderAssetSystem(ObjectManager& manager);
        ShaderAssetSystem(const ::Inscription::Table<ShaderAssetSystem>& table);
    private:
        void InitializeImpl() override;
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::ShaderAssetSystem)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}