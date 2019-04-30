
#include "TileSystem.h"

#include "ObjectManager.h"
#include "DebugStatisticsSystem.h"

#include "StringUtility.h"

namespace Atmos
{
    TileSystem::TileSystem(ObjectManager& manager) : ObjectSystem(manager)
    {
        tiles = manager.Batch<Tile>();
    }

    TileSystem::TileSystem(const ::Inscription::Table<TileSystem>& table) :
        INSCRIPTION_TABLE_GET_BASE(ObjectSystem)
    {}

    TypedObjectReference<Tile> TileSystem::FindTile(const GridPosition& at) const
    {
        for (auto& loop : tiles)
        {
            if (loop->position == at)
                return TypedObjectReference<Tile>(loop);
        }

        return TypedObjectReference<Tile>();
    }

    bool TileSystem::Exists(const GridPosition& at) const
    {
        return FindTile(at).IsOccupied();
    }

    ObjectBatchSizeT TileSystem::Size() const
    {
        return tiles.Size();
    }

    void TileSystem::InitializeImpl()
    {
        auto debugStatistics = Manager()->FindSystem<DebugStatisticsSystem>();
        debugStatistics->memoryPage.tileSize.retrievalFunction = [this]() -> String
        {
            return ToString(Size());
        };
    }
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::TileSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::TileSystem, "TileSystem");
}