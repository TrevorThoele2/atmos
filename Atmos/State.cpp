#include "State.h"

#include "ObjectManager.h"

#include "StateSystem.h"

namespace Atmos::State
{
    State::~State()
    {}

    void State::Initialize()
    {
        InitializeImpl();
    }

    void State::Work()
    {
        WorkImpl();
    }

    void State::Goto()
    {
        System()->Goto(*this);
    }

    bool State::CanGoto() const
    {
        return DoCanGoto();
    }

    void State::PopIfTop()
    {
        if (!IsTop())
            return;

        System()->Pop();
    }

    bool State::IsTop() const
    {
        return System()->Top() == this;
    }

    State::State(ObjectManager& manager) : Object(manager)
    {}

    State::State(const ::Inscription::BinaryTableData<State>& data) :
        Object(std::get<0>(data.bases))
    {}

    StateSystem* State::System()
    {
        return Manager()->FindSystem<StateSystem>();
    }

    const StateSystem* State::System() const
    {
        return Manager()->FindSystem<StateSystem>();
    }

    bool State::DoCanGoto() const
    {
        return true;
    }

    void State::OnFocused()
    {
        DoOnFocused();
    }

    void State::OnUnfocused()
    {
        Hide();
        DoOnUnfocused();
    }

    void State::DoOnFocused()
    {}

    void State::DoOnUnfocused()
    {}
}

namespace Atmos
{
    const ObjectTypeName ObjectTraits<State::State>::typeName = "State";
}