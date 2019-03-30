
#include "nModulatorSystem.h"

#include "ObjectManager.h"

namespace Atmos
{
    namespace Modulator
    {
        System::System(ObjectManager& manager) : ObjectSystem(manager)
        {
            modulatedObjects = manager.Batch<ModulatedObject>();
            modulatedObjects.onCreated.Subscribe(&System::OnModulatedObjectCreated, *this);
            modulatedObjects.onBeforeDestroyed.Subscribe(&System::OnModulatedObjectDestroyed, *this);
        }

        System::ModulatedObjectReference System::ModulatedObjectFor(ConstObjectReference baseObject)
        {
            auto found = modulatedObjectMap.find(baseObject);
            if (found == modulatedObjectMap.end())
                return ModulatedObjectReference();

            return found->second;
        }

        bool System::IsModulated(ConstObjectReference baseObject)
        {
            return ModulatedObjectFor(baseObject).IsOccupied();
        }

        void System::OnModulatedObjectCreated(ModulatedObjectReference reference)
        {
            modulatedObjectMap.emplace(reference->source.Get(), reference);
        }

        void System::OnModulatedObjectDestroyed(ModulatedObjectReference reference)
        {
            modulatedObjectMap.erase(reference->source.Get());
        }
    }
}