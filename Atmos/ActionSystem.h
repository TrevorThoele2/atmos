#pragma once

#include "EntitySystem.h"

namespace Atmos
{
    class Field;
    namespace Ent
    {
        class ActionSystem : public System<ActionSystem>
        {
        private:
            typedef System<ActionSystem> SystemBaseT;
            friend SystemBaseT;
        private:
            ActionSystem();

            void Init() override final;
            static void OnFieldSet(Field &field);
        public:
        };
    }
}