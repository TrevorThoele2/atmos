
#include "nEntityPositionSystem.h"

#include "ObjectManager.h"

#include "nEntitySystem.h"

namespace Atmos
{
    namespace Ent
    {
        nEntityPositionSystem::nEntityPositionSystem(ObjectManager& manager) : ObjectSystem(manager)
        {
            entities = manager.Batch<nEntity>();
            entities.onCreated.Subscribe(&nEntityPositionSystem::OnEntityCreated, *this);
            entities.onBeforeDestroyed.Subscribe(&nEntityPositionSystem::OnEntityDestroyed, *this);
        }

        void nEntityPositionSystem::SetDirection(EntityReference entity, const Direction& direction)
        {

        }

        void nEntityPositionSystem::MoveEntity(EntityReference entity, const GridPosition& to, TimeValue timeTaken)
        {

        }

        void nEntityPositionSystem::MoveEntity(EntityReference entity, const Direction& direction)
        {

        }

        void nEntityPositionSystem::MoveEntityInstant(EntityReference entity, const GridPosition& to)
        {

        }

        void nEntityPositionSystem::MoveEntityInstant(EntityReference entity, const RelativeGridPosition& offset)
        {

        }

        bool nEntityPositionSystem::CanMove(ConstEntityReference entity) const
        {
            return false;
        }

        bool nEntityPositionSystem::CanMove(ConstEntityReference entity, const GridPosition& to) const
        {
            return false;
        }

        bool nEntityPositionSystem::CanMove(ConstEntityReference entity, const RelativeGridPosition& to) const
        {
            return false;
        }

        bool nEntityPositionSystem::CanMove(ConstEntityReference entity, const Direction& direction) const
        {
            return false;
        }

        bool nEntityPositionSystem::IsMoving(ConstEntityReference entity) const
        {
            return std::find(movingEntities.begin(), movingEntities.end(), entity) != movingEntities.end();
        }

        ObjectBatchSizeT nEntityPositionSystem::MovingSize() const
        {
            return movingEntities.size();
        }

        void nEntityPositionSystem::OnEntityCreated(EntityReference reference)
        {
            auto& generalComponent = GeneralComponentFor(reference);
            positionedEntities.emplace(generalComponent->position, reference);
        }

        void nEntityPositionSystem::OnEntityDestroyed(EntityReference reference)
        {
            auto& generalComponent = GeneralComponentFor(reference);
            positionedEntities.erase(generalComponent->position);
        }

        nEntityPositionSystem::GeneralComponentReference nEntityPositionSystem::GeneralComponentFor(EntityReference entity)
        {
            return entity->Component<nGeneralComponent>();
        }
    }
}