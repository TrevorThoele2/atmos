
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

        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(ActionSystem) : INSCRIPTION_TABLE_GET_BASE(ObjectSystem)
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
    INSCRIPTION_BINARY_INSCRIPTER_DEFINE_TABLE(::Atmos::Entity::ActionSystem)
    {
        INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_BINARY_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::Entity::ActionSystem, "EntityActionSystem");
}