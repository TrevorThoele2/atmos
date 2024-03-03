#pragma once

#include "ObjectSystem.h"
#include "ObjectBatch.h"
#include "ObjectReference.h"

#include "Tile.h"

#include "ObjectScribe.h"

namespace Atmos
{
    class TileSystem : public ObjectSystem
    {
    public:
        typedef TypedObjectReference<Tile> Reference;
    public:
        TileSystem(ObjectManager& manager);

        Reference FindTile(const GridPosition& at) const;

        bool Exists(const GridPosition& at) const;

        ObjectBatchSizeT Size() const;
    private:
        void InitializeImpl() override;
    private:
        typedef ObjectBatch<Tile> TileBatch;
        TileBatch tiles;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::TileSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::TileSystem, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}