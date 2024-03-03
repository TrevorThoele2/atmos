
#include "ShaderAssetSystem.h"

#include "DebugStatisticsSystem.h"

#include "StringUtility.h"

namespace Atmos
{
    ShaderAssetSystem::ShaderAssetSystem(ObjectManager& manager) : AssetSystem(manager)
    {}

    ShaderAssetSystem::ShaderAssetSystem(const ::Inscription::Table<ShaderAssetSystem>& table) : INSCRIPTION_TABLE_GET_BASE(AssetSystem)
    {}

    void ShaderAssetSystem::InitializeImpl()
    {
        auto debugStatistics = Manager()->FindSystem<DebugStatisticsSystem>();
        debugStatistics->memoryPage.shaderAssetSize.retrievalFunction = [this]() -> String
        {
            return ToString(Size());
        };
    }
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::ShaderAssetSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::AssetSystem<::Atmos::ShaderAsset>);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::ShaderAssetSystem, "ShaderAssetSystem");
}