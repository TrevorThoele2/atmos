#pragma once

#include "ObjectSystem.h"
#include "ObjectBatch.h"
#include "ObjectReference.h"

#include "Entity.h"
#include "GeneralComponent.h"

#include "GridPosition.h"

namespace Atmos::Entity
{
    class PositionSystem : public ObjectSystem
    {
    private:
        typedef TypedObjectReference<Entity> EntityReference;
        typedef ConstTypedObjectReference<Entity> ConstEntityReference;
    public:
        PositionSystem(ObjectManager& manager);

        void SetDirection(EntityReference entity, const Direction& direction);

        void MoveEntity(EntityReference entity, const Grid::Position& to, Time::Value timeTaken);
        void MoveEntity(EntityReference entity, const Direction& direction);
        void MoveEntityInstant(EntityReference entity, const Grid::Position& to);
        void MoveEntityInstant(EntityReference entity, const Grid::RelativePosition& offset);

        bool CanMove(ConstEntityReference entity) const;
        bool CanMove(ConstEntityReference entity, const Grid::Position& to) const;
        bool CanMove(ConstEntityReference entity, const Grid::RelativePosition& to) const;
        bool CanMove(ConstEntityReference entity, const Direction& direction) const;

        bool IsMoving(ConstEntityReference entity) const;

        ObjectBatchSizeT MovingSize() const;
    protected:
        void InitializeImpl() override;
    private:
        typedef ObjectBatch<Entity> EntityBatch;
        EntityBatch entities;

        void OnEntityCreated(EntityReference reference);
        void OnEntityDestroyed(EntityReference reference);
    private:
        typedef std::unordered_map<Grid::Position, EntityReference> PositionedEntities;
        PositionedEntities positionedEntities;
    private:
        typedef std::vector<EntityReference> MovingEntities;
        MovingEntities movingEntities;
    private:
        typedef TypedObjectReference<GeneralComponent> GeneralComponentReference;
        GeneralComponentReference GeneralComponentFor(EntityReference entity);
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Entity::PositionSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::Entity::PositionSystem, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}