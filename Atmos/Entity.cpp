#include "Entity.h"

#include "DataCore.h"

#include "MappedEntities.h"
#include "IsSolid.h"

namespace Atmos::Entity
{
    Entity::Entity(Arca::RelicInit init, Arca::Serialization)
    {
        init.Create<DataCore>();
    }

    Entity::Entity(
        Arca::RelicInit init,
        Name name,
        Name displayName,
        Spatial::Grid::Point position,
        Spatial::Angle2D direction,
        bool isSolid)
        :
        name(name),
        displayName(displayName),
        position(position),
        direction(direction),
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
        const Atmos::Name& displayName,
        const Atmos::Spatial::Grid::Point& position,
        const Atmos::Spatial::Angle2D& direction,
        bool isSolid)
    {
        const auto mappedEntities = Arca::Index<Atmos::Entity::Mapped>(reliquary);
        if (mappedEntities->nameToEntity.find(name) != mappedEntities->nameToEntity.end())
            return false;

        if (isSolid && reliquary.Do(Atmos::World::IsSolid{ position }))
            return false;

        return true;
    }
}