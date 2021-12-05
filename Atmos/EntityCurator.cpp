#include "EntityCurator.h"

#include "EntityAlgorithms.h"
#include "CurrentActualizingEntity.h"
#include "Map.h"
#include "DataAlgorithms.h"

#include <Arca/Reliquary.h>
#include <Arca/Created.h>
#include <Arca/Destroying.h>

namespace Atmos::Entity
{
    Curator::Curator(Init init) :
        Arca::Curator(init),
        entities(init.owner.Batch<Entity>()),
        mapped(init.owner.Find<Mapped>())
    {
        Owner().On<Arca::CreatedKnown<Entity>>(
            [this](const Arca::CreatedKnown<Entity>& signal)
            {
                const auto index = signal.index;

                auto mutableMappedEntities = MutablePointer().Of(mapped);
                AddEntityTo(mutableMappedEntities->nameToEntity, index->name, index);
                AddEntityTo(mutableMappedEntities->positionToEntity, index->position, index);
            });

        Owner().On<Arca::DestroyingKnown<Entity>>(
            [this](const Arca::DestroyingKnown<Entity>& signal)
            {
                const auto index = signal.index;

                auto mutableMappedEntities = MutablePointer().Of(mapped);
                RemoveEntityFrom(mutableMappedEntities->nameToEntity, index);
                RemoveEntityFrom(mutableMappedEntities->positionToEntity, index);
            });
    }

    Arca::Index<Entity> Curator::Handle(const FindByName& command)
    {
        const auto mappedEntities = Owner().Find<Mapped>();
        auto& nameToEntity = mappedEntities->nameToEntity;
        const auto found = nameToEntity.find(command.name);
        if (found != nameToEntity.end())
            return found->second;
        else
            return {};
    }

    std::set<Arca::Index<Entity>> Curator::Handle(const FindByPosition& command)
    {
        const auto mappedEntities = Owner().Find<Mapped>();
        auto& positionToEntity = mappedEntities->positionToEntity;
        const auto found = positionToEntity.find(command.position);
        if (found != positionToEntity.end())
            return found->second;
        else
            return {};
    }

    void Curator::Handle(const MoveTo& command)
    {
        const auto entity = Owner().Find<Atmos::Entity::Entity>(command.entity);

        if (!DoCanMoveTo(entity->isSolid, command.to, Owner()))
            return;

        auto mutableMappedEntities = MutablePointer().Of(mapped);

        auto& positionToEntity = mutableMappedEntities->positionToEntity;
        RemoveEntityFrom(positionToEntity, entity);

        auto mutableEntity = MutablePointer().Of(entity);
        mutableEntity->position = command.to;

        AddEntityTo(positionToEntity, command.to, entity);
    }

    bool Curator::Handle(const CanMoveTo& command)
    {
        const auto entity = Owner().Find<Atmos::Entity::Entity>(command.entity);
        return DoCanMoveTo(entity->isSolid, command.to, Owner());
    }

    void Curator::Handle(const ModifyTags& command)
    {
        const auto entity = Owner().Find<Atmos::Entity::Entity>(command.entity);
        auto& tags = MutablePointer().Of(entity)->tags;
        ApplyAddRemoveModifications(tags, command.add, command.remove);
    }

    Path Curator::Handle(const FindPath& command)
    {
        const auto entity = Owner().Find<Atmos::Entity::Entity>(command.entity);
        return pathfinder.FindPath(entity, command.to, Owner());
    }

    void Curator::AddEntityTo(Mapped::NameToEntity& to, const String& name, Arca::Index<Entity> entity)
    {
        to.emplace(name, entity);
    }

    void Curator::AddEntityTo(Mapped::PositionToEntity& to, const Spatial::Grid::Point& position, Arca::Index<Entity> entity)
    {
        auto value = to.find(position);
        if (value == to.end())
            value = to.emplace(position, std::set<Arca::Index<Entity>>{}).first;
        value->second.emplace(entity);
    }

    void Curator::RemoveEntityFrom(Mapped::NameToEntity& from, Arca::Index<Entity> entity)
    {
        from.erase(entity->name);
    }

    void Curator::RemoveEntityFrom(Mapped::PositionToEntity& from, Arca::Index<Entity> entity)
    {
        auto set = from.find(entity->position);
        set->second.erase(entity);
        if (set->second.empty())
            from.erase(set);
    }
}