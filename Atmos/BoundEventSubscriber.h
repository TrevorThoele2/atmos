#pragma once

#include <memory>

#include "ScopedEventConnection.h"
#include "Event.h"

namespace Atmos
{
    class BoundEventSubscriber
    {
    public:
        BoundEventSubscriber() = default;
        template<class Ret, class... Args>
        BoundEventSubscriber(Event<Args...>& subTo, Ret(*func)(Args...));
        template<class Obj, class Ret, class... Args>
        BoundEventSubscriber(Event<Args...>& subTo, Ret(Obj::*func)(Args...), Obj& obj);
        template<class Obj, class Ret, class... Args>
        BoundEventSubscriber(Event<Args...>& subTo, Ret(Obj::*func)(Args...) const, const Obj& obj);

        template<class Ret, class... Args>
        void Setup(Event<Args...>& subTo, Ret(*func)(Args...));
        template<class Obj, class Ret, class... Args>
        void Setup(Event<Args...>& subTo, Ret(Obj::*func)(Args...), Obj& obj);
        template<class Obj, class Ret, class... Args>
        void Setup(Event<Args...>& subTo, Ret(Obj::*func)(Args...) const, const Obj& obj);

        void Subscribe();
        void Unsubscribe();

        bool IsValid() const;
    private:
        class Base
        {
        public:
            ScopedEventConnection connection;

            virtual ~Base() = 0;
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
        public:
            Derived(EventT* owner, FunctionT&& function);
            void Sub() override;
        };
    private:
        std::unique_ptr<Base> base;
    };

    template<class... Args>
    BoundEventSubscriber::Derived<Args...>::Derived(EventT* owner, FunctionT&& function) : owner(owner), function(std::move(function))
    {}

    template<class... Args>
    void BoundEventSubscriber::Derived<Args...>::Sub()
    {
        connection.Set<Args...>(owner->Subscribe(function));
    }

    template<class Ret, class... Args>
    BoundEventSubscriber::BoundEventSubscriber(Event<Args...>& subTo, Ret(*func)(Args...)) :
        base(new Derived<Args...>(&subTo, ::Chroma::CreateFunction(func)))
    {}

    template<class Obj, class Ret, class... Args>
    BoundEventSubscriber::BoundEventSubscriber(Event<Args...>& subTo, Ret(Obj::*func)(Args...), Obj& obj) :
        base(new Derived<Args...>(&subTo, ::Chroma::CreateFunction(func, obj)))
    {}

    template<class Obj, class Ret, class... Args>
    BoundEventSubscriber::BoundEventSubscriber(Event<Args...>& subTo, Ret(Obj::*func)(Args...) const, const Obj& obj) :
        base(new Derived<Args...>(&subTo, ::Chroma::CreateFunction(func, obj)))
    {}

    template<class Ret, class... Args>
    void BoundEventSubscriber::Setup(Event<Args...>& subTo, Ret(*func)(Args...))
    {
        base.reset(new Derived<Args...>(&subTo, std::function<void(Args...)>(func)));
    }

    template<class Obj, class Ret, class... Args>
    void BoundEventSubscriber::Setup(Event<Args...>& subTo, Ret(Obj::*func)(Args...), Obj& obj)
    {
        base.reset(new Derived<Args...>(&subTo, ::Chroma::CreateFunction(func, obj)));
    }

    template<class Obj, class Ret, class... Args>
    void BoundEventSubscriber::Setup(Event<Args...>& subTo, Ret(Obj::*func)(Args...) const, const Obj& obj)
    {
        base.reset(new Derived<Args...>(&subTo, ::Chroma::CreateFunction(func, obj)));
    }
}