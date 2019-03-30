
#include "TileFactory.h"

#include "TileSystem.h"

#include "ObjectManager.h"

namespace Atmos
{
    ObjectFactory<nTile>::ObjectFactory(ObjectManager& manager) : ObjectFactoryBase(manager)
    {}

    ObjectFactory<nTile>::Created* ObjectFactory<nTile>::CreateObject(const GridPosition& position)
    {
        auto found = Manager()->FindSystem<TileSystem>()->FindTile(position);
        if (found)
            return nullptr;

        return new Created(position);
    }
}