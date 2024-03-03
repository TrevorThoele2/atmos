
#include "ImageAssetSystem.h"

#include "DebugStatisticsSystem.h"

#include "StringUtility.h"

namespace Atmos
{
    ImageAssetSystem::ImageAssetSystem(ObjectManager& manager) : AssetSystem(manager)
    {}

    ImageAssetSystem::ImageAssetSystem(const ::Inscription::Table<ImageAssetSystem>& table) : INSCRIPTION_TABLE_GET_BASE(AssetSystem)
    {}

    void ImageAssetSystem::InitializeImpl()
    {
        auto debugStatistics = Manager()->FindSystem<DebugStatisticsSystem>();
        debugStatistics->memoryPage.imageAssetSize.retrievalFunction = [this]() -> String
        {
            return ToString(Size());
        };
    }
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::ImageAssetSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::AssetSystem<::Atmos::ImageAsset>);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::ImageAssetSystem, "ImageAssetSystem");
}