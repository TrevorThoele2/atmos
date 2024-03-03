
#include "StateSystem.h"

namespace Atmos
{
    StateSystem::StateSystem(ObjectManager& manager) :
        ObjectSystem(manager),
        pop(false), requested(nullptr),
        stack(Stack::FunctionPtr(Stack::MakeFunction(&State::OnUnfocused)), Stack::FunctionPtr(Stack::MakeFunction(&State::OnFocused)))
    {}


    StateSystem::StateSystem(const ::Inscription::Table<StateSystem>& table) :
        INSCRIPTION_TABLE_GET_BASE(ObjectSystem)
    {}

    void StateSystem::RegisterState(StatePtr&& state)
    {
        registered.emplace(TypeFor(*state.get()), std::move(state));
    }

    void StateSystem::InitializeStates()
    {
        for (auto& loop : registered)
            loop.second->Initialize();
    }

    void StateSystem::Work()
    {
        stack.Top()->Work();
    }

    void StateSystem::LockIn()
    {
        auto changeStack = [this](const std::function<void()>& stackChanger)
        {
            HideTopRoot();
            stackChanger();
            ShowTopRoot();

            requested = nullptr;
            pop = false;
        };

        if (pop)
        {
            auto stackChanger = [this]() { stack.Pop(); };
            changeStack(stackChanger);
        }
        else if (requested)
        {
            auto stackChanger = [this]() { stack.MakeTop(requested); };
            changeStack(stackChanger);
        }
    }

    void StateSystem::Goto(State& state)
    {
        auto found = FindRegistered(TypeFor(state));
        if (found == nullptr)
            return;

        requested = found;
    }

    void StateSystem::Pop()
    {
        pop = true;
    }

    State* StateSystem::Top()
    {
        return stack.Top();
    }

    const State* StateSystem::Top() const
    {
        return stack.Top();
    }

    bool StateSystem::HasAny() const
    {
        return !stack.empty();
    }

    State* StateSystem::FindRegistered(const std::type_index& type)
    {
        auto found = registered.find(type);
        if (found == registered.end())
            return nullptr;

        return found->second.get();
    }

    void StateSystem::HideTopRoot()
    {
        if (stack.size() != 0)
            stack.Top()->Hide();
    }

    void StateSystem::ShowTopRoot()
    {
        if (stack.size() != 0)
            stack.Top()->Show();
    }

    std::type_index StateSystem::TypeFor(State& state)
    {
        return std::type_index(typeid(state));
    }
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::StateSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::StateSystem, "StateSystem");
}