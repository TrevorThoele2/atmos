#pragma once

#include "ObjectFactory.h"
#include "Tile.h"

namespace Atmos
{
    template<>
    class ObjectFactory<Tile> : public ObjectFactoryBase
    {
    public:
        typedef Tile Created;
    public:
        ObjectFactory(ObjectManager& manager);

        Created* CreateObject(const GridPosition& position);
    };
}