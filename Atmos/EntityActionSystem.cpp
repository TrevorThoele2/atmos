
#include "EntityActionSystem.h"

#include "RunningScript.h"

#include "WorldSystem.h"

namespace Atmos
{
    namespace Entity
    {
        ActionSystem::ActionSystem(ObjectManager& manager) : ObjectSystem(manager)
        {
            actionComponents = manager.Batch<ActionComponent>();
        }

        ActionSystem::ActionSystem(const ::Inscription::Table<ActionSystem>& table) : INSCRIPTION_TABLE_GET_BASE(ObjectSystem)
        {}

        void ActionSystem::InitializeImpl()
        {
            auto worldSystem = Manager()->FindSystem<WorldSystem>();
            worldSystem->Get()->eventFieldSet.Subscribe(&ActionSystem::OnFieldSet, *this);
            worldSystem->Get()->eventFinalizeField.Subscribe(&ActionSystem::OnFieldUnset, *this);
        }

        void ActionSystem::WorkImpl()
        {
            for (auto& loop : Manager()->Batch<ActionComponent>())
            {
                auto runningScript = loop->script->RunningForThis();
                if (!runningScript.IsOccupied())
                    continue;

                runningScript->Resume();
            }
        }

        void ActionSystem::OnFieldSet(Field& field)
        {
            for (auto& loop : actionComponents)
                loop->FireFieldEntered();
        }

        void ActionSystem::OnFieldUnset(Field* field)
        {
            if (!field)
                return;

            for (auto& loop : actionComponents)
                loop->FireFieldLeft();
        }
    }
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::Entity::ActionSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::Entity::ActionSystem, "EntityActionSystem");
}