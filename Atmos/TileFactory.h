#pragma once

#include "ObjectFactory.h"
#include "Tile.h"

namespace Atmos
{
    template<>
    class ObjectFactory<nTile> : public ObjectFactoryBase
    {
    public:
        typedef nTile Created;
    public:
        ObjectFactory(ObjectManager& manager);

        Created* CreateObject(const GridPosition& position);
    };
}