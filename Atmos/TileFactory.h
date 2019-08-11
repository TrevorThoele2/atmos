#pragma once

#include "ObjectFactory.h"
#include "Tile.h"

namespace Atmos
{
    template<>
    class ObjectFactory<Grid::Tile> : public ObjectFactoryBase
    {
    public:
        typedef Grid::Tile Created;
    public:
        ObjectFactory(ObjectManager& manager);

        Created* CreateObject(const Grid::Position& position);
    };
}