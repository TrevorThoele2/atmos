#pragma once

#include <memory>
#include <unordered_map>
#include <typeindex>

#include "ObjectSystem.h"
#include "ObjectManager.h"

#include "UniqueStack.h"
#include "State.h"
#include "Event.h"

namespace Atmos
{
    class StateSystem : public ObjectSystem
    {
    public:
        typedef std::unique_ptr<State> StatePtr;
    public:
        Event<State&> eventTopChanged;
    public:
        StateSystem(ObjectManager& manager);
        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(StateSystem);

        void RegisterState(StatePtr&& state);
        void InitializeStates();

        void Work();
        void LockIn();

        void Goto(State& state);
        template<class NewState>
        void Goto();
        void Pop();

        State* Top();
        const State* Top() const;
        bool HasAny() const;
    private:
        State* requested;
        bool pop;
        typedef UniqueStack<State> Stack;
        Stack stack;
    private:
        std::unordered_map<std::type_index, StatePtr> registered;
        State* FindRegistered(const std::type_index& type);
    private:
        void HideTopRoot();
        void ShowTopRoot();
    private:
        std::type_index TypeFor(State& state);
        template<class StateWithGui>
        std::type_index TypeFor();
    };

    template<class NewState>
    void StateSystem::Goto()
    {
        static_assert(std::is_base_of<State, NewState>::value, "The state to goto must be derived from State.");

        auto newState = Manager()->Batch<NewState>();
        ATMOS_ASSERT(newState.Size() == 1);
        Goto(**newState.begin());
    }

    template<class StateWithGui>
    std::type_index StateSystem::TypeFor()
    {
        return std::type_index(typeid(StateWithGui));
    }

    template<>
    struct ObjectSystemTraits<StateSystem>
    {
        static const ObjectSystemPriority priority = 0;
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::StateSystem)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };
}