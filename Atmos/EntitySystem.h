#pragma once

#include <unordered_map>

#include "ObjectSystem.h"
#include "ObjectBatch.h"

#include "Entity.h"
#include "EntityComponent.h"
#include "GeneralComponent.h"

namespace Atmos::Entity
{
    class EntitySystem : public ObjectSystem
    {
    public:
        using EntityReference = TypedObjectReference<Entity>;
    public:
        EntitySystem(ObjectManager& manager);

        EntityReference EntityWithName(const Name& name) const;

        ObjectBatchSizeT Size() const;
    protected:
        void InitializeImpl() override;
    private:
        ObjectBatch<Entity> entityBatch;
        ObjectBatch<GeneralComponent> generalBatch;
    private:
        using ComponentReference = TypedObjectReference<Component>;
        ObjectBatch<Component> componentBatch;

        void OnEntityComponentCreated(ComponentReference created);
        void OnEntityComponentDestroyed(ComponentReference destroyed);
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Entity::EntitySystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::Entity::EntitySystem, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}