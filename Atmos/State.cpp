
#include "State.h"
#include "StateManager.h"

namespace Atmos
{
    StateBase::EventBound::EventBound(EventBound &&arg) : state(arg.state), base(std::move(arg.base)), connection(std::move(arg.connection))
    {}

    void StateBase::Work()
    {
        WorkInternal();
    }

    void StateBase::OnFocusedImpl()
    {}

    void StateBase::OnUnfocused()
    {
        Hide();
        OnUnfocusedImpl();
    }

    void StateBase::OnUnfocusedImpl()
    {}

    bool StateBase::CanGotoImpl() const
    {
        return true;
    }

    StateBase::StateBase()
    {
        StateManager::Instance().states.push_back(this);
    }

    void StateBase::Goto()
    {
        StateManager::Goto(*this);
    }

    bool StateBase::CanGoto() const
    {
        return CanGotoImpl();
    }

    void StateBase::Pop()
    {
        if (!IsTop())
            return;

        StateManager::Pop();
    }

    bool StateBase::IsTop() const
    {
        return &StateManager::GetTop() == &*this;
    }
}