
#include "EntitySystem.h"

#include "ObjectManager.h"
#include "DebugStatisticsSystem.h"

#include "StringUtility.h"

namespace Atmos
{
    namespace Entity
    {
        System::System(ObjectManager& manager) : ObjectSystem(manager)
        {
            entityBatch = manager.Batch<Entity>();

            generalBatch = manager.Batch<GeneralComponent>();

            componentBatch = manager.Batch<Component>();
            componentBatch.onCreated.Subscribe(&System::OnEntityComponentCreated, *this);
            componentBatch.onBeforeDestroyed.Subscribe(&System::OnEntityComponentDestroyed, *this);
        }

        System::System(const ::Inscription::Table<System>& table) : INSCRIPTION_TABLE_GET_BASE(ObjectSystem)
        {}

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
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::Entity::System)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::Entity::System, "EntitySystem");
}