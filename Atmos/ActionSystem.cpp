
#include "ActionSystem.h"
#include "ActionComponent.h"
#include "WorldManager.h"

namespace Atmos
{
    namespace Ent
    {
        ActionSystem::ActionSystem()
        {}

        void ActionSystem::Init()
        {
            SubscribeEvent(WorldManager::Instance().eventFieldSet, &ActionSystem::OnFieldSet);
        }

        void ActionSystem::OnFieldSet(Field &field)
        {
            auto &actionMap = field.entities.GetMap<ActionComponent>();
            for (auto loop : actionMap)
                loop.second.OnFieldEntered();
        }

        ENTITY_SYSTEM_FORCE_INSTANTIATION(ActionSystem);
    }
}