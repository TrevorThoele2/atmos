#include "Tile.h"

#include <Inscription/VectorScribe.h>

namespace Atmos::Grid
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
}

namespace Atmos
{
    const ObjectTypeName ObjectTraits<Grid::Tile>::typeName = "Tile";
}

namespace Inscription
{
    Scribe<::Atmos::Grid::Tile, BinaryArchive>::Table::Table()
    {
        MergeDataEntries({
            DataEntry::Auto(&ObjectT::_position, &DataT::position),
            DataEntry::Auto(&ObjectT::solid, &DataT::solid),
            DataEntry::Auto(&ObjectT::_spriteList, &DataT::spriteList) });
    }
}