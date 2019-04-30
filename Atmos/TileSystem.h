#pragma once

#include "ObjectSystem.h"
#include "ObjectBatch.h"
#include "ObjectReference.h"

#include "Tile.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    class TileSystem : public ObjectSystem
    {
    public:
        typedef TypedObjectReference<Tile> Reference;
    public:
        TileSystem(ObjectManager& manager);
        TileSystem(const ::Inscription::Table<TileSystem>& table);

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
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::TileSystem)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}