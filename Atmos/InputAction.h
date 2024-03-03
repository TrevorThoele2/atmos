#pragma once

#include "InputKeyID.h"
#include "InputActionID.h"

#include "String.h"

namespace Atmos
{
    namespace Input
    {
        class SignalBase;
        class Manager;

        class Action
        {
        public:
            const ActionID id;
            const String displayName;
        public:
            Action(Manager& owner, ActionID id, const String& displayName);

            void MapToSignal(const SignalBase* set);
            const SignalBase* MappedSignal() const;
        private:
            Manager* owner;
        private:
            const SignalBase* mappedSignal;
        private:
            void Work();
        private:
            friend class Manager;
        };
    }
}