
#pragma once

#include <unordered_map>

#include "UniqueStack.h"
#include "State.h"
#include "Event.h"

namespace Atmos
{
    class StateManager
    {
    private:
        std::vector<StateBase*> states;

        bool pop;
        StateBase *requested;
        typedef UniqueStack<StateBase> Stack;
        Stack stack;

        StateManager();
        StateManager(const StateManager &arg) = delete;
        StateManager& operator=(const StateManager &arg) = delete;

        void HideTopRoot();
        void ShowTopRoot();

        void SendStateChange();

        friend StateBase;
    public:
        static Event<const StateBase&> eventNewState;

        static StateManager& Instance();
        static void Init();
        static void Work();

        static void Goto(StateBase &push);
        static void Pop();
        static void LockIn();
        static const StateBase& GetTop();
    };
}