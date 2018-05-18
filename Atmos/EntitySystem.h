#pragma once

#include <vector>
#include <list>

#include "Event.h"
#include "Optional.h"

#include <Function\Any.h>

namespace Atmos
{
    namespace Ent
    {
        class SystemHandler;
        class SystemBase
        {
        protected:
            struct ApprovedStates
            {
                bool mainGame;
                bool battle;
                ApprovedStates(bool mainGame = true, bool battle = true);
                bool Approval() const;
            };
        private:
            friend SystemHandler;
            virtual void Init();
        protected:
            bool IsCurrentStateApproved() const;
            virtual ApprovedStates GetApprovedStates() const;
        public:
            virtual ~SystemBase() = 0 {}
        };

        template<class Mixin>
        class System : public SystemBase
        {
        private:
            class EventBound
            {
            private:
                template<class... Args>
                class Derived
                {
                public:
                    typedef ::function::Function<void, Args...> Wrapped;
                    Wrapped wrapped;
                    Derived(Wrapped &&wrapped);
                    void Execute(Args ... args);
                };
            private:
                ::function::Any base;
            public:
                System *system;
                EventScopedConnection connection;
                Optional<ApprovedStates> approvedStatesOverride;

                template<class... Args>
                EventBound(System *system, ::function::Function<void, Args...> &&wrapped);
                template<class... Args>
                EventBound(System *system, ::function::Function<void, Args...> &&wrapped, const ApprovedStates &statesOverride);
                EventBound(EventBound &&arg);
                template<class... Args>
                void AttemptExecute(Args ... args);
            };

            typedef std::list<EventBound> EventBoundList;
        protected:
            typedef typename EventBoundList::iterator eventIterator;
        private:
            static Mixin instance;

            EventBoundList events;

            System(const System &arg) = delete;
            System& operator=(const System &arg) = delete;

            template<class... Args>
            eventIterator SubscribeEventCommon(function::Event<Args...> &e, EventBound &&eBound);
        protected:
            System();
            // Will not execute the function unless the current state is an approved one
            template<class Ret, class... Args>
            eventIterator SubscribeEvent(function::Event<Args...> &e, Ret(*func)(Args...));
            // Will not execute the function unless the current state is an approved one
            template<class Ret, class Obj, class... Args>
            eventIterator SubscribeEvent(function::Event<Args...> &e, Ret(Obj::*func)(Args...), Obj &obj);
            // Will not execute the function unless the current state is an approved one
            template<class Ret, class Obj, class... Args>
            eventIterator SubscribeEvent(function::Event<Args...> &e, Ret(Obj::*func)(Args...) const, const Obj &obj);
            // Will not execute the function unless the current state is an approved one
            // Overrides the approved states
            template<class Ret, class... Args>
            eventIterator SubscribeEvent(function::Event<Args...> &e, Ret(*func)(Args...), const ApprovedStates &approvedStatesOverride);
            // Will not execute the function unless the current state is an approved one
            // Overrides the approved states
            template<class Ret, class Obj, class... Args>
            eventIterator SubscribeEvent(function::Event<Args...> &e, Ret(Obj::*func)(Args...), Obj &obj, const ApprovedStates &approvedStatesOverride);
            // Will not execute the function unless the current state is an approved one
            // Overrides the approved states
            template<class Ret, class Obj, class... Args>
            eventIterator SubscribeEvent(function::Event<Args...> &e, Ret(Obj::*func)(Args...) const, const Obj &obj, const ApprovedStates &approvedStatesOverride);
        public:
            static Mixin& Instance();
        };

        template<class Mixin>
        template<class... Args>
        System<Mixin>::EventBound::Derived<Args...>::Derived(Wrapped &&wrapped) : wrapped(std::move(wrapped))
        {}

        template<class Mixin>
        template<class... Args>
        void System<Mixin>::EventBound::Derived<Args...>::Execute(Args ... args)
        {
            wrapped.Execute(std::forward<Args>(args)...);
        }

        template<class Mixin>
        template<class... Args>
        System<Mixin>::EventBound::EventBound(System *system, function::Function<void, Args...> &&wrapped) : system(system), base(Derived<Args...>(std::move(wrapped)))
        {}

        template<class Mixin>
        template<class... Args>
        System<Mixin>::EventBound::EventBound(System *system, function::Function<void, Args...> &&wrapped, const ApprovedStates &statesOverride) : system(system), base(Derived<Args...>(std::move(wrapped))), approvedStatesOverride(statesOverride)
        {}

        template<class Mixin>
        System<Mixin>::EventBound::EventBound(EventBound &&arg) : system(arg.system), base(std::move(arg.base)), connection(std::move(arg.connection)), approvedStatesOverride(std::move(arg.approvedStatesOverride))
        {}

        template<class Mixin>
        template<class... Args>
        void System<Mixin>::EventBound::AttemptExecute(Args ... args)
        {
            auto executer = [&]()
            {
                base.As<Derived<Args...>>().Execute(std::forward<Args>(args)...);
            };

            if (approvedStatesOverride.IsValid())
            {
                if (approvedStatesOverride->Approval())
                    executer();
            }
            else if (system->IsCurrentStateApproved())
                executer();
        }

        template<class Mixin>
        Mixin System<Mixin>::instance;

        template<class Mixin>
        System<Mixin>::System()
        {
            SystemHandler::Instance().systems.push_back(&instance);
        }

        template<class Mixin>
        template<class... Args>
        typename System<Mixin>::eventIterator System<Mixin>::SubscribeEventCommon(function::Event<Args...> &e, EventBound &&eBound)
        {
            events.push_back(std::move(eBound));
            auto &made = events.back();
            made.connection.Set<Args...>(e.Subscribe(&EventBound::AttemptExecute<Args...>, made));

            return --events.end();
        }

        template<class Mixin>
        template<class Ret, class... Args>
        typename System<Mixin>::eventIterator System<Mixin>::SubscribeEvent(function::Event<Args...> &e, Ret(*func)(Args...))
        {
            return SubscribeEventCommon(e, EventBound(this, function::Function<void, Args...>(func)));
        }

        template<class Mixin>
        template<class Ret, class Obj, class... Args>
        typename System<Mixin>::eventIterator System<Mixin>::SubscribeEvent(function::Event<Args...> &e, Ret(Obj::*func)(Args...), Obj &obj)
        {
            return SubscribeEventCommon(e, EventBound(this, function::Function<void, Args...>(func, obj)));
        }

        template<class Mixin>
        template<class Ret, class Obj, class... Args>
        typename System<Mixin>::eventIterator System<Mixin>::SubscribeEvent(function::Event<Args...> &e, Ret(Obj::*func)(Args...) const, const Obj &obj)
        {
            return SubscribeEventCommon(e, EventBound(this, function::Function<void, Args...>(func, obj)));
        }

        template<class Mixin>
        template<class Ret, class... Args>
        typename System<Mixin>::eventIterator System<Mixin>::SubscribeEvent(function::Event<Args...> &e, Ret(*func)(Args...), const ApprovedStates &approvedStatesOverride)
        {
            return SubscribeEventCommon(e, EventBound(this, function::Function<void, Args...>(func), approvedStatesOverride));
        }

        template<class Mixin>
        template<class Ret, class Obj, class... Args>
        typename System<Mixin>::eventIterator System<Mixin>::SubscribeEvent(function::Event<Args...> &e, Ret(Obj::*func)(Args...), Obj &obj, const ApprovedStates &approvedStatesOverride)
        {
            return SubscribeEventCommon(e, EventBound(this, function::Function<void, Args...>(func, obj), approvedStatesOverride));
        }

        template<class Mixin>
        template<class Ret, class Obj, class... Args>
        typename System<Mixin>::eventIterator System<Mixin>::SubscribeEvent(function::Event<Args...> &e, Ret(Obj::*func)(Args...) const, const Obj &obj, const ApprovedStates &approvedStatesOverride)
        {
            return SubscribeEventCommon(e, EventBound(this, function::Function<void, Args...>(func, obj), approvedStatesOverride));
        }

        template<class Mixin>
        Mixin& System<Mixin>::Instance()
        {
            return instance;
        }

        class SystemHandler
        {
        private:
            template<class Mixin>
            friend class System;

            std::vector<SystemBase*> systems;
            bool ignoreStateApproval;

            SystemHandler();
            SystemHandler(const SystemHandler &arg) = delete;
            SystemHandler& operator=(const SystemHandler &arg) = delete;

            static SystemHandler& Instance();
        public:
            static void Init();
            // If this is set, then systems will ignore their state approval
            static void IgnoreStateApproval(bool set);
            static bool IgnoreStateApproval();
        };
    }
}

#define ENTITY_SYSTEM_FORCE_INSTANTIATION(DerivedSystemT) template class System<DerivedSystemT>