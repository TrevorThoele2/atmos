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
        bool isSolid,
        const std::vector<Tag>& tags)
        :
        name(name),
        position(position),
        isSolid(isSolid),
        tags(tags)
    {
        init.Create<DataCore>();
    }

    bool Entity::operator==(const Entity& arg) const
    {
        return name == arg.name && position == arg.position && isSolid == arg.isSolid;
    }

    bool Entity::operator!=(const Entity& arg) const
    {
        return !(*this == arg);
    }
}

namespace Arca
{
    bool Traits<Atmos::Entity::Entity>::ShouldCreate(
        Reliquary& reliquary,
        const Atmos::Name& name,
        const Atmos::Spatial::Grid::Point& position,
        bool isSolid,
        const std::vector<Atmos::Tag>& tags)
    {
        const auto mappedEntities = reliquary.Find<Atmos::Entity::Mapped>();
        if (mappedEntities->nameToEntity.find(name) != mappedEntities->nameToEntity.end())
            return false;

        if (!Atmos::Entity::DoCanMoveTo(isSolid, position, reliquary))
            return false;

        return true;
    }
}