
#include "MaterialAssetSystem.h"

#include "DebugStatisticsSystem.h"

#include "StringUtility.h"

namespace Atmos
{
    MaterialAssetSystem::MaterialAssetSystem(ObjectManager& manager) : AssetSystem(manager)
    {}

    MaterialAssetSystem::MaterialAssetSystem(const ::Inscription::Table<MaterialAssetSystem>& table) : INSCRIPTION_TABLE_GET_BASE(AssetSystem)
    {}

    void MaterialAssetSystem::InitializeImpl()
    {
        auto debugStatistics = Manager()->FindSystem<DebugStatisticsSystem>();
        debugStatistics->memoryPage.materialAssetSize.retrievalFunction = [this]() -> String
        {
            return ToString(Size());
        };
    }
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::MaterialAssetSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::AssetSystem<::Atmos::MaterialAsset>);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::MaterialAssetSystem, "MaterialAssetSystem");
}