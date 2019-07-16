#pragma once

#include <unordered_map>

#include "ObjectSystem.h"
#include "ObjectBatch.h"

#include "Entity.h"
#include "EntityComponent.h"
#include "GeneralComponent.h"

namespace Atmos::Entity
{
    class System : public ObjectSystem
    {
    public:
        typedef TypedObjectReference<Entity> EntityReference;
    public:
        System(ObjectManager& manager);

        EntityReference EntityWithName(const Name& name) const;

        ObjectBatchSizeT Size() const;
    private:
        void InitializeImpl() override;
    private:
        ObjectBatch<Entity> entityBatch;
    private:
        ObjectBatch<GeneralComponent> generalBatch;
    private:
        typedef TypedObjectReference<Component> EntityComponentReference;
        ObjectBatch<Component> componentBatch;

        void OnEntityComponentCreated(EntityComponentReference created);
        void OnEntityComponentDestroyed(EntityComponentReference destroyed);
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Entity::System, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::Entity::System, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}