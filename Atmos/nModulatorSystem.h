#pragma once

#include "ObjectSystem.h"
#include "ObjectBatch.h"

#include "nModulatedObject.h"

namespace Atmos
{
    namespace Modulator
    {
        class System : public ObjectSystem
        {
        public:
            typedef TypedObjectReference<ModulatedObject> ModulatedObjectReference;
        public:
            System(ObjectManager& manager);

            ModulatedObjectReference ModulatedObjectFor(ConstObjectReference baseObject);
            bool IsModulated(ConstObjectReference baseObject);
        private:
            typedef ObjectBatch<ModulatedObject> ModulatedObjectBatch;
            ModulatedObjectBatch modulatedObjects;
            typedef std::unordered_map<ConstObjectReference, ModulatedObjectReference> ModulatedObjectMap;
            ModulatedObjectMap modulatedObjectMap;

            void OnModulatedObjectCreated(ModulatedObjectReference reference);
            void OnModulatedObjectDestroyed(ModulatedObjectReference reference);
        };
    }
}