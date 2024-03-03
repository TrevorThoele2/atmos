
#include "State.h"

#include "ObjectManager.h"

#include "StateSystem.h"

namespace Atmos
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

    State::State(const ::Inscription::Table<State>& table) : INSCRIPTION_TABLE_GET_BASE(Object)
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

    const ObjectTypeName ObjectTraits<State>::typeName = "State";
}

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::State)
    {

    }
}