
#include "ActionComponent.h"

namespace Atmos
{
    namespace Ent
    {
        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(ActionComponent)
        {
            scribe(activator);
            scribe(act);
        }

        ActionComponent::ActionComponent() : activator(Activator::USE_ON)
        {}

        ActionComponent::ActionComponent(ActionComponent &&arg) : activator(std::move(arg.activator)), act(std::move(arg.act))
        {}

        ActionComponent& ActionComponent::operator=(ActionComponent &&arg)
        {
            activator = std::move(arg.activator);
            act = std::move(arg.act);
            return *this;
        }

        void ActionComponent::OnMovedInto()
        {
            if (activator == Activator::ENTER_TILE)
                act.Execute();
        }

        void ActionComponent::OnUseOnTop()
        {
            if (activator == Activator::USE_ON)
                act.Execute();
        }

        void ActionComponent::OnFieldEntered()
        {
            if (activator == Activator::ENTER_FIELD)
                act.Execute();
        }
    }
}