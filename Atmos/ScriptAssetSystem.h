#pragma once

#include "AssetSystem.h"

#include "ScriptAsset.h"

namespace Atmos
{
    class ScriptAssetSystem : public AssetSystem<ScriptAsset>
    {
    public:
        ScriptAssetSystem(ObjectManager& manager);
        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(ScriptAssetSystem);
    private:
        void InitializeImpl() override;
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::ScriptAssetSystem)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };
}