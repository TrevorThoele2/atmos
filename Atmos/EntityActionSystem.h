#pragma once

#include "ObjectSystem.h"
#include "ObjectManager.h"

#include "ActionComponent.h"

#include "Field.h"

namespace Atmos
{
    namespace Entity
    {
        class ActionSystem : public ObjectSystem
        {
        public:
            ActionSystem(ObjectManager& manager);
            ActionSystem(const ::Inscription::Table<ActionSystem>& table);
        private:
            void InitializeImpl() override;
            void WorkImpl() override;
        private:
            ObjectBatch<ActionComponent> actionComponents;
        private:
            void OnFieldSet(Field& field);
            void OnFieldUnset(Field* field);
        };
    }
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::Entity::ActionSystem)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}