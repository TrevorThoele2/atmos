
#include "Tile.h"

#include "WorldManager.h"

#include <Inscription/Scribe.h>
#include <Inscription/Inscripter.h>
#include <Inscription/Vector.h>

namespace Atmos
{
    nTile::nTile(const GridPosition& position) :
        position(position), sprites([this]() -> SpriteList& { return _spriteList; }), solid(false)
    {}

    nTile::nTile(const ::Inscription::Table<nTile>& table) :
        INSCRIPTION_TABLE_GET_BASE(Object), INSCRIPTION_TABLE_GET_MEM(position),
        INSCRIPTION_TABLE_GET_MEM(_spriteList), INSCRIPTION_TABLE_GET_MEM(solid),
        sprites([this]() -> SpriteList& { return _spriteList; })
    {}

    ObjectTypeDescription nTile::TypeDescription() const
    {
        return ObjectTraits<nTile>::TypeDescription();
    }

    const ObjectTypeName ObjectTraits<nTile>::typeName = "Tile";
}

namespace Inscription
{
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::nTile)
    {
        INSCRIPTION_TABLE_ADD(position);
        INSCRIPTION_TABLE_ADD(_spriteList);
        INSCRIPTION_TABLE_ADD(solid);
    }
}