#pragma once

#include <unordered_map>

#include "ObjectSystem.h"
#include "ObjectBatch.h"

#include "Entity.h"
#include "EntityComponent.h"
#include "GeneralComponent.h"

namespace Atmos
{
    namespace Entity
    {
        class System : public ObjectSystem
        {
        public:
            typedef TypedObjectReference<Entity> EntityReference;
        public:
            System(ObjectManager& manager);
            System(const ::Inscription::Table<System>& table);

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
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::Entity::System)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}