
#include "ActionSystem.h"

#include "RunningScript.h"

#include "WorldManager.h"
#include "CurrentField.h"
#include "GameEnvironment.h"

namespace Atmos
{
    namespace Ent
    {
        nEntityActionSystem::nEntityActionSystem(ObjectManager& manager) : ObjectSystem(manager)
        {}

        void nEntityActionSystem::InitializeImpl()
        {
            GameEnvironment::GetWorldManager().eventFieldSet.Subscribe(&nEntityActionSystem::OnFieldSet);
            GameEnvironment::GetWorldManager().eventFinalizeField.Subscribe(&nEntityActionSystem::OnFieldUnset);
        }

        void nEntityActionSystem::WorkImpl()
        {
            for (auto& loop : GetLocalObjectManager()->Batch<nActionComponent>())
            {
                auto runningScript = loop->script->RunningForThis();
                if (!runningScript.IsOccupied())
                    continue;

                runningScript->Resume();
            }
        }

        void nEntityActionSystem::OnFieldSet(Field& field)
        {
            auto actionComponents = AllActionComponents(field);
            for (auto& loop : actionComponents)
                loop->FireFieldEntered();
        }

        void nEntityActionSystem::OnFieldUnset(Field* field)
        {
            if (!field)
                return;

            auto actionComponents = AllActionComponents(*field);
            for (auto& loop : actionComponents)
                loop->FireFieldLeft();
        }

        ObjectBatch<nActionComponent> nEntityActionSystem::AllActionComponents(Field& field)
        {
            return field.objectManager.Batch<nActionComponent>();
        }
    }
}