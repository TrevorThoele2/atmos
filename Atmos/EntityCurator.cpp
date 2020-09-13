#include "EntityCurator.h"

#include "RunningScript.h"
#include "FieldSet.h"
#include "FieldUnset.h"

#include <Arca/Reliquary.h>
#include <Arca/Created.h>
#include <Arca/Destroying.h>

namespace Atmos::Entity
{
    Curator::Curator(Init init) :
        Arca::Curator(init),
        prototypes(init.owner.Batch<Prototype>()),
        entities(init.owner.Batch<Entity>()),
        mappedEntities(init.owner)
    {
        Owner().On<Arca::CreatedKnown<Entity>>(
            [this](const Arca::CreatedKnown<Entity>& signal)
            {
                auto mutableMappedEntities = MutablePointer().Of(mappedEntities);
                mutableMappedEntities->nameToEntity.emplace(signal.index->name, signal.index);
                mutableMappedEntities->positionToEntity.emplace(signal.index->position, signal.index);
            });

        Owner().On<Arca::DestroyingKnown<Entity>>(
            [this](const Arca::DestroyingKnown<Entity>& signal)
            {
                auto mutableMappedEntities = MutablePointer().Of(mappedEntities);
                mutableMappedEntities->nameToEntity.erase(signal.index->name);
                mutableMappedEntities->positionToEntity.erase(signal.index->position);
            });
    }

    void Curator::Work()
    {

    }

    void Curator::Handle(const ActualizeAllPrototypes&)
    {
        struct ToInitialize
        {
            Entity* entity;
            Script::Instance* initializer;
            ToInitialize(Entity* entity, Script::Instance* initializer) :
                entity(entity), initializer(initializer)
            {}
        };

        std::vector<ToInitialize> toInitialize;
        for (auto& prototype : prototypes)
        {
            auto entity = MutablePointer().Of(Owner().Do(
                Arca::Create<Entity>(prototype.name, prototype.position, prototype.direction)));
            if (!prototype.initializer)
                continue;
            auto initializer = MutablePointer().Of(prototype.initializer);
            toInitialize.emplace_back(entity, initializer);
        }

        for(auto& currentCreated : toInitialize)
        {
            currentCreated.initializer->ExecuteImmediately();
        }

        Owner().Do(Arca::Clear(Arca::TypeFor<Prototype>()));
    }

    void Curator::Handle(const Move& command)
    {
        auto mutableMappedEntities = MutablePointer().Of(mappedEntities);
        auto mutableEntity = MutablePointer().Of(command.entity);

        mutableMappedEntities->positionToEntity.erase(mutableEntity->position);
        mutableEntity->position = command.toPosition;
        mutableMappedEntities->positionToEntity.emplace(command.toPosition, command.entity);
    }
}