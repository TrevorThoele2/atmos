
#pragma once

#include "StateGui.h"

#include "Event.h"
#include <Function\Any.h>

namespace Atmos
{
    class StateManager;
    class StateBase
    {
    private:
        class EventBound
        {
        private:
            template<class... Args>
            class Derived
            {
            public:
                typedef function::Function<void, Args...> Wrapped;
                Wrapped wrapped;
                Derived(Wrapped &&wrapped);
                void Execute(Args ... args);
            };
        private:
            function::Any base;
        public:
            StateBase *state;
            EventScopedConnection connection;

            template<class... Args>
            EventBound(StateBase *state, function::Function<void, Args...> &&wrapped);
            EventBound(EventBound &&arg);
            template<class... Args>
            void AttemptExecute(Args ... args);
        };

        typedef std::list<EventBound> EventBoundList;
    protected:
        typedef typename EventBoundList::iterator eventIterator;
    private:
        friend StateManager;
    private:
        EventBoundList events;

        void Work();
        virtual void WorkInternal() = 0;
        virtual void OnFocusedImpl();
        void OnUnfocused();
        virtual void OnUnfocusedImpl();
        virtual void Show() = 0;
        virtual void Hide() = 0;

        virtual bool CanGotoImpl() const;

        template<class... Args>
        eventIterator SubscribeEventCommon(function::Event<Args...> &e, EventBound &&eBound);
    protected:
        // Will not execute the function unless the current state is the top one
        template<class Ret, class... Args>
        eventIterator SubscribeEvent(function::Event<Args...> &e, Ret(*func)(Args...));
        // Will not execute the function unless the current state is the top one
        template<class Ret, class Obj, class... Args>
        eventIterator SubscribeEvent(function::Event<Args...> &e, Ret(Obj::*func)(Args...), Obj &obj);
        // Will not execute the function unless the current state is the top one
        template<class Ret, class Obj, class... Args>
        eventIterator SubscribeEvent(function::Event<Args...> &e, Ret(Obj::*func)(Args...) const, const Obj &obj);
    public:
        StateBase();
        StateBase(const StateBase &arg) = delete;
        StateBase& operator=(const StateBase &arg) = delete;
        virtual void Init() = 0;
        void Goto();
        bool CanGoto() const;
        // Only works if this is the top state
        void Pop();
        bool IsTop() const;
    };

    template<class... Args>
    StateBase::EventBound::Derived<Args...>::Derived(Wrapped &&wrapped) : wrapped(std::move(wrapped))
    {}

    template<class... Args>
    void StateBase::EventBound::Derived<Args...>::Execute(Args ... args)
    {
        wrapped.Execute(std::forward<Args>(args)...);
    }

    template<class... Args>
    StateBase::EventBound::EventBound(StateBase *state, function::Function<void, Args...> &&wrapped) : state(state), base(Derived<Args...>(std::move(wrapped)))
    {}

    template<class... Args>
    void StateBase::EventBound::AttemptExecute(Args ... args)
    {
        if(state->IsTop())
            base.As<Derived<Args...>>().Execute(std::forward<Args>(args)...);
    }

    template<class... Args>
    StateBase::eventIterator StateBase::SubscribeEventCommon(function::Event<Args...> &e, EventBound &&eBound)
    {
        events.push_back(std::move(eBound));
        auto &made = events.back();
        made.connection.Set<Args...>(e.Subscribe(&EventBound::AttemptExecute<Args...>, made));

        return --events.end();
    }

    template<class Ret, class... Args>
    StateBase::eventIterator StateBase::SubscribeEvent(function::Event<Args...> &e, Ret(*func)(Args...))
    {
        return SubscribeEventCommon(e, EventBound(this, function::Function<void, Args...>(func)));
    }

    template<class Ret, class Obj, class... Args>
    StateBase::eventIterator StateBase::SubscribeEvent(function::Event<Args...> &e, Ret(Obj::*func)(Args...), Obj &obj)
    {
        return SubscribeEventCommon(e, EventBound(this, function::Function<void, Args...>(func, obj)));
    }

    template<class Ret, class Obj, class... Args>
    StateBase::eventIterator StateBase::SubscribeEvent(function::Event<Args...> &e, Ret(Obj::*func)(Args...) const, const Obj &obj)
    {
        return SubscribeEventCommon(e, EventBound(this, function::Function<void, Args...>(func, obj)));
    }

    template<class Gui>
    class State : public StateBase
    {
    private:
        void WorkInternal() override final;
        virtual void WorkImpl();
        virtual void InitImpl();
        void Show() override final;
        void Hide() override final;
    public:
        Gui gui;
        void Init() override final;
        void Update();
    };

    template<class Gui>
    void State<Gui>::WorkInternal()
    {
        WorkImpl();
        gui.Update();
    }

    template<class Gui>
    void State<Gui>::WorkImpl()
    {}

    template<class Gui>
    void State<Gui>::InitImpl()
    {}

    template<class Gui>
    void State<Gui>::Show()
    {
        gui.Show();
    }

    template<class Gui>
    void State<Gui>::Hide()
    {
        gui.Hide();
    }

    template<class Gui>
    void State<Gui>::Init()
    {
        InitImpl();
        gui.Init();
    }

    template<class Gui>
    void State<Gui>::Update()
    {
        gui.Update();
    }
}