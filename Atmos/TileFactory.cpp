#include "TileFactory.h"

#include "TileSystem.h"

#include "ObjectManager.h"

namespace Atmos
{
    ObjectFactory<Grid::Tile>::ObjectFactory(ObjectManager& manager) : ObjectFactoryBase(manager)
    {}

    ObjectFactory<Grid::Tile>::Created* ObjectFactory<Grid::Tile>::CreateObject(const Grid::Position& position)
    {
        auto found = Manager()->FindSystem<Grid::TileSystem>()->FindTile(position);
        if (found)
            return nullptr;

        return new Created(*Manager(), position);
    }
}