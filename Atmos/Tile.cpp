
#include "Tile.h"

#include <Inscription/Scribe.h>
#include <Inscription/Inscripter.h>
#include <Inscription/Vector.h>

namespace Atmos
{
    Tile::Tile(ObjectManager& manager, const GridPosition& position) :
        Object(manager),
        position(position), sprites([this]() -> SpriteList& { return _spriteList; }), solid(false)
    {}

    Tile::Tile(const ::Inscription::Table<Tile>& table) :
        INSCRIPTION_TABLE_GET_BASE(Object), INSCRIPTION_TABLE_GET_MEM(position),
        INSCRIPTION_TABLE_GET_MEM(_spriteList), INSCRIPTION_TABLE_GET_MEM(solid),
        sprites([this]() -> SpriteList& { return _spriteList; })
    {}

    ObjectTypeDescription Tile::TypeDescription() const
    {
        return ObjectTraits<Tile>::TypeDescription();
    }

    const ObjectTypeName ObjectTraits<Tile>::typeName = "Tile";
}

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::Tile)
    {
        INSCRIPTION_TABLE_ADD(position);
        INSCRIPTION_TABLE_ADD(_spriteList);
        INSCRIPTION_TABLE_ADD(solid);
    }
}