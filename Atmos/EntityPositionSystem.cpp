#include "EntityPositionSystem.h"

#include "ObjectManager.h"
#include "EntitySystem.h"
#include "DebugStatisticsSystem.h"

#include "StringUtility.h"

namespace Atmos::Entity
{
    PositionSystem::PositionSystem(ObjectManager& manager) : ObjectSystem(manager)
    {
        entities = manager.Batch<Entity>();
        entities.onCreated.Subscribe(&PositionSystem::OnEntityCreated, *this);
        entities.onBeforeDestroyed.Subscribe(&PositionSystem::OnEntityDestroyed, *this);
    }

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

namespace Inscription
{
    void Scribe<::Atmos::Entity::PositionSystem, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::ObjectSystem>(object, archive);
    }
}