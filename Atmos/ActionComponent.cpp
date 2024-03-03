#include "ActionComponent.h"

namespace Atmos::Entity
{
    void ActionComponent::FireMovedInto() const
    {
        if (activation == Activation::EnterTile)
            script->ExecuteDeferred();
    }

    void ActionComponent::FireAttemptMovedInto() const
    {
        if (activation == Activation::AttemptEnterTile)
            script->ExecuteDeferred();
    }

    void ActionComponent::FireUseOn() const
    {
        if (activation == Activation::UseOn)
            script->ExecuteDeferred();
    }

    void ActionComponent::FireUseInto() const
    {
        if (activation == Activation::UseInto)
            script->ExecuteDeferred();
    }

    void ActionComponent::FireFieldEntered() const
    {
        if (activation == Activation::EnterField)
            script->ExecuteDeferred();
    }

    void ActionComponent::FireFieldLeft() const
    {
        if (activation == Activation::LeaveField)
            script->ExecuteDeferred();
    }
}

namespace Arca
{
    const TypeName Traits<::Atmos::Entity::ActionComponent>::typeName = "ActionComponent";
}