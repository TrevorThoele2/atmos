#pragma once

#include "ObjectSystem.h"
#include "ObjectManager.h"

#include "ActionComponent.h"

#include "Field.h"

namespace Atmos
{
    namespace Ent
    {
        class nEntityActionSystem : public ObjectSystem
        {
        public:
            nEntityActionSystem(ObjectManager& manager);
        private:
            void InitializeImpl() override;
            void WorkImpl() override;
        private:
            static void OnFieldSet(Field& field);
            static void OnFieldUnset(Field* field);
        private:
            static ObjectBatch<nActionComponent> AllActionComponents(Field& field);
        };
    }
}