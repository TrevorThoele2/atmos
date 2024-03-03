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
            INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(System);

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
        INSCRIPTION_BINARY_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };
}