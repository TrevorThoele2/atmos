#pragma once

#include <unordered_map>

#include "ObjectSystem.h"
#include "ObjectBatch.h"

#include "nEntity.h"
#include "GeneralComponent.h"
#include "nEntityComponent.h"

namespace Atmos
{
    namespace Ent
    {
        class nEntitySystem : public ObjectSystem
        {
        public:
            typedef TypedObjectReference<nEntity> EntityReference;
        public:
            nEntitySystem(ObjectManager& manager);

            EntityReference EntityWithName(const Name& name) const;

            ObjectBatchSizeT Size() const;
        private:
            ObjectBatch<nEntity> entityBatch;
        private:
            ObjectBatch<nGeneralComponent> generalBatch;
        private:
            typedef TypedObjectReference<nEntityComponent> EntityComponentReference;
            ObjectBatch<nEntityComponent> componentBatch;

            void OnEntityComponentCreated(EntityComponentReference created);
            void OnEntityComponentDestroyed(EntityComponentReference destroyed);
        };
    }
}