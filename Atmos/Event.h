
#pragma once

#include <memory>
#include <list>
#include <unordered_set>

#include <Function\Event.h>

namespace Atmos
{
    class EventAnyConnection
    {
    private:
        class Base
        {
        public:
            virtual ~Base() = 0 {}
            virtual Base* Clone() const = 0;
            virtual void Sever() = 0;
            virtual bool IsValid() const = 0;
            virtual bool Compare(const EventAnyConnection &arg) const = 0;
        };

        template<class... Args>
        class Derived : public Base
        {
        public:
            typedef ::function::EventConnection<Args...> WrappedT;

            WrappedT wrapped;

            Derived(const WrappedT &wrapped);
            Derived(WrappedT &&wrapped);
            Derived(const Derived &arg) = default;
            Derived& operator=(const Derived &arg) = delete;

            Derived* Clone() const override final;

            void Sever() override final;
            bool IsValid() const override final;

            bool Compare(const EventAnyConnection &arg) const override final;
        };
    private:
        std::unique_ptr<Base> base;
    public:
        EventAnyConnection() = default;
        template<class... Args>
        EventAnyConnection(const ::function::EventConnection<Args...> &connection);
        template<class... Args>
        EventAnyConnection(::function::EventConnection<Args...>&&connection);
        EventAnyConnection(const EventAnyConnection &arg);
        EventAnyConnection(EventAnyConnection &&arg);
        EventAnyConnection& operator=(const EventAnyConnection &arg);
        EventAnyConnection& operator=(EventAnyConnection &&arg);

        bool operator==(const EventAnyConnection &arg) const;
        bool operator!=(const EventAnyConnection &arg) const;

        template<class... Args>
        void Set(const ::function::EventConnection<Args...> &set);
        template<class... Args>
        void Set(::function::EventConnection<Args...> &&set);

        void Sever();
        bool IsValid() const;
    };

    template<class... Args>
    EventAnyConnection::Derived<Args...>::Derived(const WrappedT &wrapped) : wrapped(wrapped)
    {}

    template<class... Args>
    EventAnyConnection::Derived<Args...>::Derived(WrappedT &&wrapped) : wrapped(std::move(wrapped))
    {}

    template<class... Args>
    typename EventAnyConnection::Derived<Args...>* EventAnyConnection::Derived<Args...>::Clone() const
    {
        return new Derived(*this);
    }

    template<class... Args>
    void EventAnyConnection::Derived<Args...>::Sever()
    {
        wrapped.Sever();
    }

    template<class... Args>
    bool EventAnyConnection::Derived<Args...>::IsValid() const
    {
        return wrapped.IsValid();
    }

    template<class... Args>
    bool EventAnyConnection::Derived<Args...>::Compare(const EventAnyConnection &arg) const
    {
        return wrapped == static_cast<Derived*>(arg.base.get())->wrapped;
    }

    template<class... Args>
    EventAnyConnection::EventAnyConnection(const ::function::EventConnection<Args...> &connection) : base(new Derived<Args...>(connection))
    {}

    template<class... Args>
    EventAnyConnection::EventAnyConnection(::function::EventConnection<Args...> &&connection) : base(new Derived<Args...>(std::move(connection)))
    {}

    template<class... Args>
    void EventAnyConnection::Set(const ::function::EventConnection<Args...> &set)
    {
        base.reset(new Derived<Args...>(set));
    }

    template<class... Args>
    void EventAnyConnection::Set(::function::EventConnection<Args...> &&set)
    {
        base.reset(new Derived<Args...>(std::move(set)));
    }

    template<class... Args>
    using Event = function::Event<Args...>;
    using NullEvent = function::NullEvent;

    // This will remove the connection when this is destructed
    class EventScopedConnection
    {
    private:
        class Base
        {
        public:
            virtual ~Base() = 0 {}
            virtual Base* Clone() const = 0;
            virtual void Sever() = 0;
            virtual bool IsValid() const = 0;
        };

        template<class... Args>
        class Derived : public Base
        {
        public:
            typedef typename function::Event<Args...>::Connection Wrapped;
            Wrapped wrapped;

            Derived(const Wrapped &wrapped);
            Derived(Wrapped &&arg);
            ~Derived();
            Derived* Clone() const override;
            void Sever() override;
            bool IsValid() const override;
        };
    private:
        std::unique_ptr<Base> base;
    public:
        EventScopedConnection() = default;
        template<class... Args>
        EventScopedConnection(const function::EventConnection<Args...> &wrapped);
        template<class... Args>
        EventScopedConnection(function::EventConnection<Args...> &&wrapped);
        EventScopedConnection(const EventScopedConnection &arg);
        EventScopedConnection(EventScopedConnection &&arg);
        ~EventScopedConnection();
        EventScopedConnection& operator=(const EventScopedConnection &arg);
        EventScopedConnection& operator=(EventScopedConnection &&arg);
        bool operator==(const EventScopedConnection &arg) const;
        bool operator!=(const EventScopedConnection &arg) const;

        template<class... Args>
        void Set(const function::EventConnection<Args...> &set);
        template<class... Args>
        void Set(function::EventConnection<Args...> &&set);
        // Does not remove this slot from the event
        void Reset();

        // This will assume that you know that the args are valid (does no checking)
        template<class... Args>
        void Execute(Args && ... args);
        // Removes slot from event
        void Sever();

        bool IsValid() const;
    };

    template<class... Args>
    EventScopedConnection::Derived<Args...>::Derived(const Wrapped &wrapped) : wrapped(wrapped)
    {}

    template<class... Args>
    EventScopedConnection::Derived<Args...>::Derived(Wrapped &&wrapped) : wrapped(std::move(wrapped))
    {}

    template<class... Args>
    EventScopedConnection::Derived<Args...>::~Derived()
    {
        Sever();
    }

    template<class... Args>
    EventScopedConnection::Derived<Args...>* EventScopedConnection::Derived<Args...>::Clone() const
    {
        return new Derived(*this);
    }

    template<class... Args>
    void EventScopedConnection::Derived<Args...>::Sever()
    {
        if (IsValid())
            wrapped.Sever();
    }

    template<class... Args>
    bool EventScopedConnection::Derived<Args...>::IsValid() const
    {
        return wrapped.IsValid();
    }

    template<class... Args>
    EventScopedConnection::EventScopedConnection(const function::EventConnection<Args...> &wrapped) : base(new Derived<Args...>(wrapped))
    {}

    template<class... Args>
    EventScopedConnection::EventScopedConnection(function::EventConnection<Args...> &&wrapped) : base(new Derived<Args...>(std::move(wrapped)))
    {}

    template<class... Args>
    void EventScopedConnection::Set(const function::EventConnection<Args...> &set)
    {
        base.reset(new Derived<Args...>(set));
    }

    template<class... Args>
    void EventScopedConnection::Set(function::EventConnection<Args...> &&set)
    {
        base.reset(new Derived<Args...>(std::move(set)));
    }

    template<class... Args>
    void EventScopedConnection::Execute(Args && ... args)
    {
        static_cast<Derived<Args...>*>(base.get())->wrapped.Execute(std::forward<Args>(args)...);
    }

    // Manages a group of auto slots
    class EventScopedConnectionManager
    {
    private:
        std::list<EventScopedConnection> slots;
    public:
        template<class... Args>
        void Add(const function::EventConnection<Args...> &connection);
        template<class... Args>
        void Add(function::EventConnection<Args...> &&connection);
        void Add(const EventScopedConnection &add);
        void Add(EventScopedConnection &&add);

        void Clear();
    };

    template<class... Args>
    void EventScopedConnectionManager::Add(const function::EventConnection<Args...> &connection)
    {
        slots.push_back(EventScopedConnection(connection));
    }

    template<class... Args>
    void EventScopedConnectionManager::Add(function::EventConnection<Args...> &&connection)
    {
        slots.push_back(EventScopedConnection(std::move(connection)));
    }

    class EventBoundSubscriber
    {
    private:
        class Base
        {
        public:
            EventScopedConnection connection;

            virtual ~Base() = 0 {}
            virtual void Sub() = 0;
        };

        template<class... Args>
        class Derived : public Base
        {
        public:
            typedef Event<Args...> EventT;
            EventT *owner;

            typedef typename EventT::FunctionT FunctionT;
            FunctionT function;

            Derived(EventT *owner, FunctionT &&function);
            void Sub() override;
        };
    private:
        std::unique_ptr<Base> base;
    public:
        EventBoundSubscriber() = default;
        template<class Ret, class... Args>
        EventBoundSubscriber(Event<Args...> &subTo, Ret(*func)(Args...));
        template<class Obj, class Ret, class... Args>
        EventBoundSubscriber(Event<Args...> &subTo, Ret(Obj::*func)(Args...), Obj &obj);
        template<class Obj, class Ret, class... Args>
        EventBoundSubscriber(Event<Args...> &subTo, Ret(Obj::*func)(Args...) const, const Obj &obj);

        template<class Ret, class... Args>
        void Setup(Event<Args...> &subTo, Ret(*func)(Args...));
        template<class Obj, class Ret, class... Args>
        void Setup(Event<Args...> &subTo, Ret(Obj::*func)(Args...), Obj &obj);
        template<class Obj, class Ret, class... Args>
        void Setup(Event<Args...> &subTo, Ret(Obj::*func)(Args...) const, const Obj &obj);

        void Subscribe();
        void Unsubscribe();

        bool IsValid() const;
    };

    template<class... Args>
    EventBoundSubscriber::Derived<Args...>::Derived(EventT *owner, FunctionT &&function) : owner(owner), function(std::move(function))
    {}

    template<class... Args>
    void EventBoundSubscriber::Derived<Args...>::Sub()
    {
        connection.Set<Args...>(owner->Subscribe(function));
    }

    template<class Ret, class... Args>
    EventBoundSubscriber::EventBoundSubscriber(Event<Args...> &subTo, Ret(*func)(Args...)) : base(new Derived<Args...>(&subTo, function::Function<void, Args...>(func)))
    {}

    template<class Obj, class Ret, class... Args>
    EventBoundSubscriber::EventBoundSubscriber(Event<Args...> &subTo, Ret(Obj::*func)(Args...), Obj &obj) : base(new Derived<Args...>(&subTo, function::Function<void, Args...>(func, obj)))
    {}

    template<class Obj, class Ret, class... Args>
    EventBoundSubscriber::EventBoundSubscriber(Event<Args...> &subTo, Ret(Obj::*func)(Args...) const, const Obj &obj) : base(new Derived<Args...>(&subTo, function::Function<void, Args...>(func, obj)))
    {}

    template<class Ret, class... Args>
    void EventBoundSubscriber::Setup(Event<Args...> &subTo, Ret(*func)(Args...))
    {
        base.reset(new Derived<Args...>(&subTo, function::Function<void, Args...>(func)));
    }

    template<class Obj, class Ret, class... Args>
    void EventBoundSubscriber::Setup(Event<Args...> &subTo, Ret(Obj::*func)(Args...), Obj &obj)
    {
        base.reset(new Derived<Args...>(&subTo, function::Function<void, Args...>(func, obj)));
    }

    template<class Obj, class Ret, class... Args>
    void EventBoundSubscriber::Setup(Event<Args...> &subTo, Ret(Obj::*func)(Args...) const, const Obj &obj)
    {
        base.reset(new Derived<Args...>(&subTo, function::Function<void, Args...>(func, obj)));
    }

    class EventBoundSubscriberManager
    {
    private:
        std::vector<EventBoundSubscriber> subs;
    public:
        template<class Ret, class... Args>
        void Add(Event<Args...> &subTo, Ret(*func)(Args...));
        template<class Obj, class Ret, class... Args>
        void Add(Event<Args...> &subTo, Ret(Obj::*func)(Args...), Obj &obj);
        template<class Obj, class Ret, class... Args>
        void Add(Event<Args...> &subTo, Ret(Obj::*func)(Args...) const, const Obj &obj);

        void Subscribe();
        void Unsubscribe();

        bool IsValid() const;
    };

    template<class Ret, class... Args>
    void EventBoundSubscriberManager::Add(Event<Args...> &subTo, Ret(*func)(Args...))
    {
        subs.push_back(EventBoundSubscriber(subTo, func));
    }

    template<class Obj, class Ret, class... Args>
    void EventBoundSubscriberManager::Add(Event<Args...> &subTo, Ret(Obj::*func)(Args...), Obj &obj)
    {
        subs.push_back(EventBoundSubscriber(subTo, func, obj));
    }

    template<class Obj, class Ret, class... Args>
    void EventBoundSubscriberManager::Add(Event<Args...> &subTo, Ret(Obj::*func)(Args...) const, const Obj &obj)
    {
        subs.push_back(EventBoundSubscriber(subTo, func, obj));
    }
}