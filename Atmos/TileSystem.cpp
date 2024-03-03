
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
    void Scribe<::Atmos::TileSystem, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::ObjectSystem>(object, archive);
    }
}