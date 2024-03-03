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

        void MoveEntity(EntityReference entity, const GridPosition& to, TimeValue timeTaken);
        void MoveEntity(EntityReference entity, const Direction& direction);
        void MoveEntityInstant(EntityReference entity, const GridPosition& to);
        void MoveEntityInstant(EntityReference entity, const RelativeGridPosition& offset);

        bool CanMove(ConstEntityReference entity) const;
        bool CanMove(ConstEntityReference entity, const GridPosition& to) const;
        bool CanMove(ConstEntityReference entity, const RelativeGridPosition& to) const;
        bool CanMove(ConstEntityReference entity, const Direction& direction) const;

        bool IsMoving(ConstEntityReference entity) const;

        ObjectBatchSizeT MovingSize() const;
    private:
        void InitializeImpl() override;
    private:
        typedef ObjectBatch<Entity> EntityBatch;
        EntityBatch entities;

        void OnEntityCreated(EntityReference reference);
        void OnEntityDestroyed(EntityReference reference);
    private:
        typedef std::unordered_map<GridPosition, EntityReference> PositionedEntities;
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
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}