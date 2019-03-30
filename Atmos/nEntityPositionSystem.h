#pragma once

#include "ObjectSystem.h"
#include "ObjectBatch.h"
#include "ObjectReference.h"

#include "nEntity.h"
#include "GeneralComponent.h"

#include "GridPosition.h"

namespace Atmos
{
    namespace Ent
    {
        class nEntityPositionSystem : public ObjectSystem
        {
        private:
            typedef TypedObjectReference<nEntity> EntityReference;
            typedef ConstTypedObjectReference<nEntity> ConstEntityReference;
        public:
            nEntityPositionSystem(ObjectManager& manager);

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
            typedef ObjectBatch<nEntity> EntityBatch;
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
            typedef TypedObjectReference<nGeneralComponent> GeneralComponentReference;
            GeneralComponentReference GeneralComponentFor(EntityReference entity);
        };
    }
}