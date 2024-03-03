
#include "EntityPositionSystem.h"

#include "ObjectManager.h"
#include "EntitySystem.h"
#include "DebugStatisticsSystem.h"

#include "StringUtility.h"

namespace Atmos
{
    namespace Entity
    {
        PositionSystem::PositionSystem(ObjectManager& manager) : ObjectSystem(manager)
        {
            entities = manager.Batch<Entity>();
            entities.onCreated.Subscribe(&PositionSystem::OnEntityCreated, *this);
            entities.onBeforeDestroyed.Subscribe(&PositionSystem::OnEntityDestroyed, *this);
        }

        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(PositionSystem) : INSCRIPTION_TABLE_GET_BASE(ObjectSystem)
        {}

        void PositionSystem::SetDirection(EntityReference entity, const Direction& direction)
        {

        }

        void PositionSystem::MoveEntity(EntityReference entity, const GridPosition& to, TimeValue timeTaken)
        {

        }

        void PositionSystem::MoveEntity(EntityReference entity, const Direction& direction)
        {

        }

        void PositionSystem::MoveEntityInstant(EntityReference entity, const GridPosition& to)
        {

        }

        void PositionSystem::MoveEntityInstant(EntityReference entity, const RelativeGridPosition& offset)
        {

        }

        bool PositionSystem::CanMove(ConstEntityReference entity) const
        {
            return false;
        }

        bool PositionSystem::CanMove(ConstEntityReference entity, const GridPosition& to) const
        {
            return false;
        }

        bool PositionSystem::CanMove(ConstEntityReference entity, const RelativeGridPosition& to) const
        {
            return false;
        }

        bool PositionSystem::CanMove(ConstEntityReference entity, const Direction& direction) const
        {
            return false;
        }

        bool PositionSystem::IsMoving(ConstEntityReference entity) const
        {
            return std::find(movingEntities.begin(), movingEntities.end(), entity) != movingEntities.end();
        }

        ObjectBatchSizeT PositionSystem::MovingSize() const
        {
            return movingEntities.size();
        }

        void PositionSystem::InitializeImpl()
        {
            auto debugStatistics = Manager()->FindSystem<DebugStatisticsSystem>();
            debugStatistics->memoryPage.movingEntitySize.retrievalFunction = [this]() -> String
            {
                return ToString(MovingSize());
            };
        }

        void PositionSystem::OnEntityCreated(EntityReference reference)
        {
            auto& generalComponent = GeneralComponentFor(reference);
            positionedEntities.emplace(generalComponent->position, reference);
        }

        void PositionSystem::OnEntityDestroyed(EntityReference reference)
        {
            auto& generalComponent = GeneralComponentFor(reference);
            positionedEntities.erase(generalComponent->position);
        }

        PositionSystem::GeneralComponentReference PositionSystem::GeneralComponentFor(EntityReference entity)
        {
            return entity->RetrieveComponent<GeneralComponent>();
        }
    }
}

namespace Inscription
{
    INSCRIPTION_BINARY_INSCRIPTER_DEFINE_TABLE(::Atmos::Entity::PositionSystem)
    {
        INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_BINARY_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::Entity::PositionSystem, "EntityPositionSystem");
}