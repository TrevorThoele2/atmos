#pragma once

#include "Input.h"
#include "InputDefines.h"
#include "IniID.h"

namespace Atmos
{
    namespace Input
    {
        class HandlerBase;
        class Action
        {
        private:
            const SignalBase *mappedKey;

            Action(ActionID id, const String &displayName, Ini::ID iniID, const SignalBase &defaultKey);
            void Work();
            friend HandlerBase;
        public:
            void SetMappedKey(const SignalBase *set);
            void SetMappedKey(KeyID set);
            const SignalBase* GetMappedKey() const;

            const ActionID id;
            const String displayName;
            const Ini::ID iniID;
        };
    }
}