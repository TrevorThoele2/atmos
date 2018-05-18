#pragma once

#include <unordered_map>

#include "EntitySystem.h"
#include "Entity.h"

#include "Name.h"

namespace Atmos
{
    class Field;
    namespace Ent
    {
        class GeneralComponent;
        class NameSystem : public System<NameSystem>
        {
        private:
            typedef System<NameSystem> SystemBaseT;
            friend SystemBaseT;

            static std::unordered_map<Name, Entity> map;

            NameSystem() = default;
            NameSystem(const NameSystem &arg) = delete;
            NameSystem& operator=(const NameSystem &arg) = delete;

            void Init() override final;
            void OnGeneralComponentCreated(GeneralComponent &component);
            void OnEntityDestroyed(Entity entity);

            void OnFinalizeField();
        public:
            static void ChangeName(const Name &from, const Name &to);
            static Entity FindEntity(const Name &name);
        };
    }
}