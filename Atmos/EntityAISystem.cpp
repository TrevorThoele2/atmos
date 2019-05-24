
#include "EntityAISystem.h"

#include "RunningScript.h"

#include "ObjectManager.h"

namespace Atmos
{
    namespace Entity
    {
        AISystem::AISystem(ObjectManager& manager) : ObjectSystem(manager)
        {
            aiComponents = manager.Batch<AIComponent>();
        }

        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(AISystem) : INSCRIPTION_TABLE_GET_BASE(ObjectSystem)
        {}

        void AISystem::WorkImpl()
        {
            for (auto& loop : aiComponents)
            {
                auto running = loop->script->RunningForThis();
                if (!running.IsOccupied())
                    continue;

                running->Resume();
            }
        }
    }
}

namespace Inscription
{
    INSCRIPTION_BINARY_INSCRIPTER_DEFINE_TABLE(::Atmos::Entity::AISystem)
    {
        INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_BINARY_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::Entity::AISystem, "EntityAISystem");
}