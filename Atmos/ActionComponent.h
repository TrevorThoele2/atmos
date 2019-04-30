#pragma once

#include "EntityComponent.h"

#include "ScriptInstance.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    namespace Entity
    {
        class ActionComponent : public Component
        {
        public:
            enum class Activator : std::int32_t
            {
                ENTER_TILE,
                ATTEMPT_ENTER_TILE,
                USE_ON,
                USE_INTO,
                ENTER_FIELD,
                LEAVE_FIELD
            };
        public:
            typedef TypedObjectReference<ScriptInstance> ScriptInstanceReference;
        public:
            Activator activator;
            ScriptInstanceReference script;

            ActionComponent(ObjectManager& manager, EntityReference owner);
            ActionComponent(const ActionComponent& arg) = default;
            ActionComponent(const ::Inscription::Table<ActionComponent>& table);

            void FireMovedInto();
            void FireAttemptMovedInto();
            void FireUseOn();
            void FireUseInto();
            void FireFieldEntered();
            void FireFieldLeft();

            ObjectTypeDescription TypeDescription() const override;
        private:
            void SetupScripts();
        private:
            INSCRIPTION_ACCESS;
        };
    }

    template<>
    struct ObjectTraits<Entity::ActionComponent> : ObjectTraitsBase<Entity::ActionComponent>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::Entity::ActionComponent)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;
    };
}