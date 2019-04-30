#pragma once

#include "AssetSystem.h"

#include "ScriptAsset.h"

namespace Atmos
{
    class ScriptAssetSystem : public AssetSystem<ScriptAsset>
    {
    public:
        ScriptAssetSystem(ObjectManager& manager);
        ScriptAssetSystem(const ::Inscription::Table<ScriptAssetSystem>& table);
    private:
        void InitializeImpl() override;
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::ScriptAssetSystem)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}