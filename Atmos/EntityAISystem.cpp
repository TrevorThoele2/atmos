
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

        AISystem::AISystem(const ::Inscription::Table<AISystem>& table) : INSCRIPTION_TABLE_GET_BASE(ObjectSystem)
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
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::Entity::AISystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::Entity::AISystem, "EntityAISystem");
}