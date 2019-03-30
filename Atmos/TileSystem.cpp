
#include "TileSystem.h"

#include "ObjectManager.h"

namespace Atmos
{
    TileSystem::TileSystem(ObjectManager& manager) : ObjectSystem(manager)
    {
        tiles = manager.Batch<nTile>();
    }

    TypedObjectReference<nTile> TileSystem::FindTile(const GridPosition& at) const
    {
        for (auto& loop : tiles)
        {
            if (loop->position == at)
                return TypedObjectReference<nTile>(loop);
        }

        return TypedObjectReference<nTile>();
    }

    bool TileSystem::Exists(const GridPosition& at) const
    {
        return FindTile(at).IsOccupied();
    }

    ObjectBatchSizeT TileSystem::Size() const
    {
        return tiles.Size();
    }
}