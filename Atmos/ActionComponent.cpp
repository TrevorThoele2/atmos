
#include "ActionComponent.h"

namespace Atmos
{
    namespace Entity
    {
        ActionComponent::ActionComponent(ObjectManager& manager, EntityReference owner) :
            Component(manager, owner), activator(Activator::USE_ON)
        {
            SetupScripts();
        }

        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(ActionComponent) : INSCRIPTION_TABLE_GET_BASE(Component)
        {
            SetupScripts();
        }

        void ActionComponent::FireMovedInto()
        {
            if (activator == Activator::ENTER_TILE)
                script->ExecuteDeferred();
        }

        void ActionComponent::FireAttemptMovedInto()
        {
            if (activator == Activator::ATTEMPT_ENTER_TILE)
                script->ExecuteDeferred();
        }

        void ActionComponent::FireUseOn()
        {
            if (activator == Activator::USE_ON)
                script->ExecuteDeferred();
        }

        void ActionComponent::FireUseInto()
        {
            if (activator == Activator::USE_INTO)
                script->ExecuteDeferred();
        }

        void ActionComponent::FireFieldEntered()
        {
            if (activator == Activator::ENTER_FIELD)
                script->ExecuteDeferred();
        }

        void ActionComponent::FireFieldLeft()
        {
            if (activator == Activator::LEAVE_FIELD)
                script->ExecuteDeferred();
        }

        ObjectTypeDescription ActionComponent::TypeDescription() const
        {
            return ObjectTraits<ActionComponent>::TypeDescription();
        }

        void ActionComponent::SetupScripts()
        {
            script->owner = this;
        }
    }

    const ObjectTypeName ObjectTraits<Entity::ActionComponent>::typeName = "ActionComponent";
}

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::Entity::ActionComponent)
    {
        INSCRIPTION_TABLE_ADD(activator);
        INSCRIPTION_TABLE_ADD(script);
    }
}