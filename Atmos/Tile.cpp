#include "Tile.h"

#include <Inscription/VectorScribe.h>

namespace Atmos
{
    Tile::Tile(ObjectManager& manager, const Position& position) :
        Object(manager), _position(position), solid(false)
    {}

    Tile::Tile(const ::Inscription::BinaryTableData<Tile>& data) :
        Object(std::get<0>(data.bases)), _position(data.position), _spriteList(data.spriteList), solid(data.solid)
    {}

    ObjectTypeDescription Tile::TypeDescription() const
    {
        return ObjectTraits<Tile>::TypeDescription();
    }

    const ObjectTypeName ObjectTraits<Tile>::typeName = "Tile";
}

namespace Inscription
{
    Scribe<::Atmos::Tile, BinaryArchive>::Table::Table()
    {
        MergeDataEntries({
            DataEntry::Auto(&ObjectT::_position, &DataT::position),
            DataEntry::Auto(&ObjectT::_spriteList, &DataT::spriteList),
            DataEntry::Auto(&ObjectT::solid, &DataT::solid) });
    }
}