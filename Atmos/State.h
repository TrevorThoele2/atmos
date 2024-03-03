#pragma once

#include "Object.h"

#include "ScopedEventConnectionList.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    class StateSystem;

    class State : public Object
    {
    public:
        virtual ~State() = 0;

        void Initialize();
        void Work();

        void Goto();
        bool CanGoto() const;
        void PopIfTop();
        bool IsTop() const;
    protected:
        State(ObjectManager& manager);
        State(const ::Inscription::Table<State>& table);

        StateSystem* System();
        const StateSystem* System() const;
    protected:
        template<class Ret, class... Args>
        void ExecuteWhenTop(::Chroma::Event<Args...>& e, Ret(*func)(Args...));
        template<class Ret, class Obj, class... Args>
        void ExecuteWhenTop(::Chroma::Event<Args...>& e, Ret(Obj::*func)(Args...), Obj& obj);
        template<class Ret, class Obj, class... Args>
        void ExecuteWhenTop(::Chroma::Event<Args...>& e, Ret(Obj::*func)(Args...) const, const Obj& obj);
    private:
        ScopedEventConnectionList eventConnections;
    private:
        virtual void InitializeImpl() = 0;
        virtual void WorkImpl() = 0;

        virtual bool DoCanGoto() const;
    private:
        virtual void Show() = 0;
        virtual void Hide() = 0;
    private:
        void OnFocused();
        void OnUnfocused();
        virtual void DoOnFocused();
        virtual void DoOnUnfocused();
    private:
        template<class... Args>
        void ExecuteWhenTopCommon(::Chroma::Event<Args...>& e, typename ::Chroma::Event<Args...>::FunctionT&& subscribingFunction);
    private:
        friend class StateSystem;
    };

    template<class Ret, class... Args>
    void State::ExecuteWhenTop(::Chroma::Event<Args...>& e, Ret(*func)(Args...))
    {
        ExecuteWhenTopCommon(e, ::Chroma::CreateFunction(func));
    }

    template<class Ret, class Obj, class... Args>
    void State::ExecuteWhenTop(::Chroma::Event<Args...>& e, Ret(Obj::*func)(Args...), Obj& obj)
    {
        ExecuteWhenTopCommon(e, ::Chroma::CreateFunction(func, obj));
    }

    template<class Ret, class Obj, class... Args>
    void State::ExecuteWhenTop(::Chroma::Event<Args...>& e, Ret(Obj::*func)(Args...) const, const Obj& obj)
    {
        ExecuteWhenTopCommon(e, ::Chroma::CreateFunction(func, obj));
    }

    template<class... Args>
    void State::ExecuteWhenTopCommon(::Chroma::Event<Args...>& e, typename ::Chroma::Event<Args...>::FunctionT&& subscribingFunction)
    {
        auto wrappingFunction = [this, subscribingFunction](Args&& ... args)
        {
            if (!IsTop())
                return;

            subscribingFunction(std::forward<Args>(args)...);
        };

        auto madeFunction = typename ::Chroma::Event<Args...>::FunctionT(wrappingFunction);
        auto newConnection = e.Subscribe(std::move(madeFunction));
        eventConnections.Add(std::move(newConnection));
    }

    template<>
    struct ObjectTraits<State> : ObjectTraitsBase<State>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::State)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;
    };
}