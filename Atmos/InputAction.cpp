
#include "InputAction.h"

#include "Environment.h"

namespace Atmos
{
    namespace Input
    {
        Action::Action(ActionID id, const String &displayName, Ini::ID iniID, const SignalBase &defaultKey) : id(id), displayName(displayName), iniID(iniID), mappedKey(&defaultKey)
        {}

        void Action::Work()
        {
            if (mappedKey->IsActive())
            {
                Environment::GetInput()->eventActionActive(*this);
                // Pressed
                if (mappedKey->IsPressed())
                    Environment::GetInput()->eventActionPressed(*this);
            }
            else if (mappedKey->IsDepressed())
                Environment::GetInput()->eventActionDepressed(*this);
        }

        void Action::SetMappedKey(const SignalBase *set)
        {
            if (!set)
                return;

            mappedKey = set;
            static_cast<Ini::Manager::InputEntry*>(Environment::GetIni().GetEntry(iniID))->value = set->AsKey()->id;
        }

        void Action::SetMappedKey(KeyID set)
        {
            auto found = Environment::GetInput()->GetKey(set);
            // If not found or you can use the found for an action, then use it
            if (!found || found->CanUseForAction())
                SetMappedKey(found);
        }

        const SignalBase* Action::GetMappedKey() const
        {
            return mappedKey;
        }
    }
}