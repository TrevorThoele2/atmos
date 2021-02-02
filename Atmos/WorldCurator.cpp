#include "WorldCurator.h"

#include "Map.h"
#include "MappedEntities.h"

#include "DataAlgorithms.h"

namespace Atmos::World
{
    Curator::Curator(Init init, Manager& manager) : Arca::Curator(init), manager(&manager)
    {}

    void Curator::Handle(const RequestField& command)
    {
        manager->Request(command.id);
    }

    void Curator::Handle(const ModifyEntityBoundary& command)
    {
        auto& entityBoundary = MutablePointer().Of<Map>()->entityBoundary;
        ApplyAddRemoveModifications(entityBoundary, command.add, command.remove);
    }

    void Curator::Handle(const ModifyProperties& command)
    {
        auto properties = manager->CurrentWorldProperties();
        ApplyPropertyModifications(properties, command.add, command.remove, command.replace);
        manager->SetWorldProperties(properties);
    }

    std::vector<Property> Curator::Handle(const RetrieveProperties& command)
    {
        return manager->CurrentWorldProperties();
    }

    bool Curator::Handle(const IsSolid& command)
    {
        auto& entities = *Arca::Index<Entity::Mapped>(Owner());
        const auto possibleEntities = entities.positionToEntity.find(command.point);
        if (possibleEntities != entities.positionToEntity.end())
        {
            const auto anyAreSolid = std::any_of(
                possibleEntities->second.begin(),
                possibleEntities->second.end(),
                [](const Arca::Index<Entity::Entity>& entity) { return entity->isSolid; });
            if (anyAreSolid)
                return true;
        }

        auto& map = *Arca::Index<Map>(Owner());
        if (map.entityBoundary.find(command.point) != map.entityBoundary.end())
            return true;

        return false;
    }
}