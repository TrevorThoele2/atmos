
#include "EntityNameSystem.h"
#include "GeneralComponent.h"

#include "CurrentField.h"
#include "WorldManager.h"

namespace Atmos
{
    namespace Ent
    {
        std::unordered_map<Name, Entity> NameSystem::map;

        void NameSystem::Init()
        {
            SubscribeEvent(GeneralComponent::Map::onCreated, &NameSystem::OnGeneralComponentCreated, *this);
            SubscribeEvent(Manager::eventBeforeEntityDestroyed, &NameSystem::OnEntityDestroyed, *this);
            SubscribeEvent(::Atmos::WorldManager::Instance().eventFinalizeField, &NameSystem::OnFinalizeField, *this);
        }

        void NameSystem::OnGeneralComponentCreated(GeneralComponent &component)
        {
            map.emplace(component.name, component.GetOwnerEntity());
        }

        void NameSystem::OnEntityDestroyed(Entity entity)
        {
            for (auto loop = map.begin(); loop != map.end(); ++loop)
            {
                if (loop->second == entity)
                {
                    map.erase(loop);
                    return;
                }
            }
        }

        void NameSystem::OnFinalizeField()
        {
            map.clear();
        }

        void NameSystem::ChangeName(const Name &from, const Name &to)
        {
            auto found = map.find(from);
            if (found == map.end())
                return;

            Entity fromEnt(found->second);
            GetCurrentEntities()->FindComponent<GeneralComponent>(fromEnt)->name = to;
            map.erase(found);
            map.emplace(to, fromEnt);
        }

        Entity NameSystem::FindEntity(const Name &name)
        {
            auto found = map.find(name);
            if (found == map.end())
                return nullEntity;

            return found->second;
        }

        ENTITY_SYSTEM_FORCE_INSTANTIATION(NameSystem);
    }
}