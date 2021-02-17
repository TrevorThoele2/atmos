#include "EntityAlgorithms.h"

#include "Map.h"
#include "MappedEntities.h"

namespace Atmos::Entity
{
    bool DoCanMoveTo(
        bool isSolid, Spatial::Grid::Point to, Arca::Reliquary& reliquary)
    {
        const auto map = Arca::Index<World::Map>(reliquary);
        if (map->entityBoundary.find(to) != map->entityBoundary.end())
            return false;

        if (!isSolid)
            return true;

        const auto entities = Arca::Index<Mapped>(reliquary);
        const auto possibleEntities = entities->positionToEntity.find(to);
        if (possibleEntities != entities->positionToEntity.end())
        {
            const auto anyAreSolid = std::any_of(
                possibleEntities->second.begin(),
                possibleEntities->second.end(),
                [](const Arca::Index<Entity>& entity) { return entity->isSolid; });
            if (anyAreSolid)
                return false;
        }

        return true;
    }
}