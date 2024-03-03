#pragma once

#include "ObjectSystem.h"
#include "ObjectBatch.h"
#include "ObjectReference.h"

#include "Tile.h"

namespace Atmos
{
    class TileSystem : public ObjectSystem
    {
    public:
        typedef TypedObjectReference<nTile> Reference;
    public:
        TileSystem(ObjectManager& manager);

        Reference FindTile(const GridPosition& at) const;

        bool Exists(const GridPosition& at) const;

        ObjectBatchSizeT Size() const;
    private:
        typedef ObjectBatch<nTile> TileBatch;
        TileBatch tiles;
    };

    template<>
    struct ObjectSystemTraits<TileSystem>
    {
        static const ObjectSystemPriority priority = 0;
    };
}