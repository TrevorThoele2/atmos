#include "Entity.h"

#include "DataCore.h"

#include "MappedEntities.h"
#include "EntityAlgorithms.h"

namespace Atmos::Entity
{
    Entity::Entity(Arca::RelicInit init, Arca::Serialization)
    {
        init.Create<DataCore>();
    }

    Entity::Entity(
        Arca::RelicInit init,
        Name name,
        Spatial::Grid::Point position,
        bool isSolid)
        :
        name(name),
        position(position),
        isSolid(isSolid)
    {
        init.Create<DataCore>();
    }
}

namespace Arca
{
    bool Traits<Atmos::Entity::Entity>::ShouldCreate(
        Reliquary& reliquary,
        const Atmos::Name& name,
        const Atmos::Spatial::Grid::Point& position,
        bool isSolid)
    {
        const auto mappedEntities = Arca::Index<Atmos::Entity::Mapped>(reliquary);
        if (mappedEntities->nameToEntity.find(name) != mappedEntities->nameToEntity.end())
            return false;

        if (!Atmos::Entity::DoCanMoveTo(isSolid, position, reliquary))
            return false;

        return true;
    }
}