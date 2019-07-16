
#include "EntitySystem.h"

#include "ObjectManager.h"
#include "DebugStatisticsSystem.h"

#include "StringUtility.h"

namespace Atmos::Entity
{
    System::System(ObjectManager& manager) : ObjectSystem(manager)
    {
        entityBatch = manager.Batch<Entity>();

        generalBatch = manager.Batch<GeneralComponent>();

        componentBatch = manager.Batch<Component>();
        componentBatch.onCreated.Subscribe(&System::OnEntityComponentCreated, *this);
        componentBatch.onBeforeDestroyed.Subscribe(&System::OnEntityComponentDestroyed, *this);
    }

    System::EntityReference System::EntityWithName(const Name& name) const
    {
        for (auto& loop : generalBatch)
            if (loop->name == name)
                return loop->owner;

        return EntityReference();
    }

    ObjectBatchSizeT System::Size() const
    {
        return generalBatch.Size();
    }

    void System::InitializeImpl()
    {
        auto debugStatistics = Manager()->FindSystem<DebugStatisticsSystem>();
        debugStatistics->memoryPage.entitySize.retrievalFunction = [this]() -> String
        {
            return ToString(Size());
        };
    }

    void System::OnEntityComponentCreated(EntityComponentReference created)
    {
        created->owner->componentList.push_back(created);
    }

    void System::OnEntityComponentDestroyed(EntityComponentReference destroyed)
    {
        for (auto loop = destroyed->owner->componentList.begin(); loop != destroyed->owner->componentList.end(); ++loop)
        {
            if (*loop == destroyed)
            {
                destroyed->owner->componentList.erase(loop);
                return;
            }
        }
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Entity::System, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::ObjectSystem>(object, archive);
    }
}