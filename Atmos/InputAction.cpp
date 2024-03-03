
#include "InputAction.h"

#include "InputManager.h"
#include "InputSignalBase.h"

namespace Atmos
{
    namespace Input
    {
        void Action::MapToSignal(const SignalBase* set)
        {
            if (!set)
                return;

            mappedSignal = set;
        }

        const SignalBase* Action::MappedSignal() const
        {
            return mappedSignal;
        }

        Action::Action(Manager& owner, ActionID id, const String& displayName) :
            owner(&owner), id(id), displayName(displayName), mappedSignal(nullptr)
        {}

        void Action::Work()
        {
            if (mappedSignal->IsActive())
            {
                owner->eventActionActive(*this);

                if (mappedSignal->IsPressed())
                    owner->eventActionPressed(*this);
            }
            else if (mappedSignal->IsDepressed())
                owner->eventActionDepressed(*this);
        }
    }
}