
#include "TileFactory.h"

#include "TileSystem.h"

#include "ObjectManager.h"

namespace Atmos
{
    ObjectFactory<Tile>::ObjectFactory(ObjectManager& manager) : ObjectFactoryBase(manager)
    {}

    ObjectFactory<Tile>::Created* ObjectFactory<Tile>::CreateObject(const GridPosition& position)
    {
        auto found = Manager()->FindSystem<TileSystem>()->FindTile(position);
        if (found)
            return nullptr;

        return new Created(*Manager(), position);
    }
}