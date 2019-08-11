#pragma once

#include "ObjectSystem.h"
#include "ObjectBatch.h"
#include "ObjectReference.h"

#include "Tile.h"

#include "ObjectScribe.h"

namespace Atmos::Grid
{
    class TileSystem : public ObjectSystem
    {
    public:
        typedef TypedObjectReference<Tile> Reference;
    public:
        TileSystem(ObjectManager& manager);

        Reference FindTile(const Position& at) const;

        bool Exists(const Position& at) const;

        ObjectBatchSizeT Size() const;
    protected:
        void InitializeImpl() override;
    private:
        typedef ObjectBatch<Tile> TileBatch;
        TileBatch tiles;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Grid::TileSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::Grid::TileSystem, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}