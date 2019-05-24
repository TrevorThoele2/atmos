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
        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(ShaderAssetSystem);
    private:
        void InitializeImpl() override;
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::ShaderAssetSystem)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };
}