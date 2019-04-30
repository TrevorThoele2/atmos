
#include "ScriptAssetSystem.h"

#include "DebugStatisticsSystem.h"

#include "StringUtility.h"

namespace Atmos
{
    ScriptAssetSystem::ScriptAssetSystem(ObjectManager& manager) : AssetSystem(manager)
    {}

    ScriptAssetSystem::ScriptAssetSystem(const ::Inscription::Table<ScriptAssetSystem>& table) : INSCRIPTION_TABLE_GET_BASE(AssetSystem)
    {}

    void ScriptAssetSystem::InitializeImpl()
    {
        auto debugStatistics = Manager()->FindSystem<DebugStatisticsSystem>();
        debugStatistics->memoryPage.scriptAssetSize.retrievalFunction = [this]() -> String
        {
            return ToString(Size());
        };
    }
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::ScriptAssetSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::AssetSystem<::Atmos::ScriptAsset>);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::ScriptAssetSystem, "ScriptAssetSystem");
}