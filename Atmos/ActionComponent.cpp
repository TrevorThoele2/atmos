
#include "ActionComponent.h"

namespace Atmos
{
    namespace Ent
    {
        nActionComponent::nActionComponent(EntityReference owner) : nEntityComponent(owner), activator(Activator::USE_ON)
        {
            SetupScripts();
        }

        nActionComponent::nActionComponent(const ::Inscription::Table<nActionComponent>& table) : INSCRIPTION_TABLE_GET_BASE(nEntityComponent)
        {
            SetupScripts();
        }

        void nActionComponent::FireMovedInto()
        {
            if (activator == Activator::ENTER_TILE)
                script->ExecuteDeferred();
        }

        void nActionComponent::FireAttemptMovedInto()
        {
            if (activator == Activator::ATTEMPT_ENTER_TILE)
                script->ExecuteDeferred();
        }

        void nActionComponent::FireUseOn()
        {
            if (activator == Activator::USE_ON)
                script->ExecuteDeferred();
        }

        void nActionComponent::FireUseInto()
        {
            if (activator == Activator::USE_INTO)
                script->ExecuteDeferred();
        }

        void nActionComponent::FireFieldEntered()
        {
            if (activator == Activator::ENTER_FIELD)
                script->ExecuteDeferred();
        }

        void nActionComponent::FireFieldLeft()
        {
            if (activator == Activator::LEAVE_FIELD)
                script->ExecuteDeferred();
        }

        ObjectTypeDescription nActionComponent::TypeDescription() const
        {
            return ObjectTraits<nActionComponent>::TypeDescription();
        }

        void nActionComponent::SetupScripts()
        {
            script->owner = this;
        }
    }

    const ObjectTypeName ObjectTraits<Ent::nActionComponent>::typeName = "ActionComponent";
}

namespace Inscription
{
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::Ent::nActionComponent)
    {
        INSCRIPTION_TABLE_ADD(activator);
        INSCRIPTION_TABLE_ADD(script);
    }
}