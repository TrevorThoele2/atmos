
#include "nEntitySystem.h"

#include "ObjectManager.h"

namespace Atmos
{
    namespace Ent
    {
        nEntitySystem::nEntitySystem(ObjectManager& manager) : ObjectSystem(manager)
        {
            entityBatch = manager.Batch<nEntity>();

            generalBatch = manager.Batch<nGeneralComponent>();

            componentBatch = manager.Batch<nEntityComponent>();
            componentBatch.onCreated.Subscribe(&nEntitySystem::OnEntityComponentCreated, *this);
            componentBatch.onBeforeDestroyed.Subscribe(&nEntitySystem::OnEntityComponentDestroyed, *this);
        }

        nEntitySystem::EntityReference nEntitySystem::EntityWithName(const Name& name) const
        {
            for (auto& loop : generalBatch)
                if (loop->name == name)
                    return loop->owner;

            return EntityReference();
        }

        ObjectBatchSizeT nEntitySystem::Size() const
        {
            return generalBatch.Size();
        }

        void nEntitySystem::OnEntityComponentCreated(EntityComponentReference created)
        {
            created->owner->componentList.push_back(created);
        }

        void nEntitySystem::OnEntityComponentDestroyed(EntityComponentReference destroyed)
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
}