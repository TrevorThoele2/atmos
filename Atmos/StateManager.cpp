
#include "StateManager.h"

#include "MainMenu.h"

namespace Atmos
{
    StateManager::StateManager() : pop(false), requested(nullptr), stack(Stack::FunctionPtr(Stack::MakeFunction(&StateBase::OnUnfocused)), Stack::FunctionPtr(Stack::MakeFunction(&StateBase::OnFocusedImpl)))
    {}

    void StateManager::HideTopRoot()
    {
        if (stack.size() != 0)
            stack.Top()->Hide();
    }

    void StateManager::ShowTopRoot()
    {
        if (stack.size() != 0)
            stack.Top()->Show();
    }

    void StateManager::SendStateChange()
    {
        eventNewState(*stack.Top());

        requested = nullptr;
        pop = false;
    }

    Event<const StateBase&> StateManager::eventNewState;

    StateManager& StateManager::Instance()
    {
        static StateManager instance;
        return instance;
    }

    void StateManager::Init()
    {
        auto &states = Instance().states;
        for (auto loop = states.begin(); loop != states.end(); loop++)
            (*loop)->Init();

        mainMenu.Goto();
        LockIn();
    }

    void StateManager::Work()
    {
        Instance().stack.Top()->Work();
    }

    void StateManager::Goto(StateBase &push)
    {
        Instance().requested = &push;
    }

    void StateManager::Pop()
    {
        Instance().pop = true;
    }

    void StateManager::LockIn()
    {
        if (Instance().pop)
        {
            Instance().HideTopRoot();
            Instance().stack.Pop();
            Instance().ShowTopRoot();

            Instance().SendStateChange();
        }
        else if (Instance().requested)
        {
            Instance().HideTopRoot();
            Instance().stack.MakeTop(Instance().requested);
            Instance().ShowTopRoot();

            Instance().SendStateChange();
        }
    }

    const StateBase& StateManager::GetTop()
    {
        return *Instance().stack.Top();
    }
}