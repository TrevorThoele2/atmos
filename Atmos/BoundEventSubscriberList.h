#pragma once

#include <vector>

#include "BoundEventSubscriber.h"
#include "Event.h"

namespace Atmos
{
    class EventBoundSubscriberList
    {
    public:
        template<class Ret, class... Args>
        void Add(Event<Args...>& subTo, Ret(*func)(Args...));
        template<class Obj, class Ret, class... Args>
        void Add(Event<Args...>& subTo, Ret(Obj::*func)(Args...), Obj& obj);
        template<class Obj, class Ret, class... Args>
        void Add(Event<Args...>& subTo, Ret(Obj::*func)(Args...) const, const Obj& obj);

        void Subscribe();
        void Unsubscribe();

        bool IsValid() const;
    private:
        std::vector<BoundEventSubscriber> subs;
    };

    template<class Ret, class... Args>
    void EventBoundSubscriberList::Add(Event<Args...>& subTo, Ret(*func)(Args...))
    {
        subs.push_back(BoundEventSubscriber(subTo, func));
    }

    template<class Obj, class Ret, class... Args>
    void EventBoundSubscriberList::Add(Event<Args...>& subTo, Ret(Obj::*func)(Args...), Obj& obj)
    {
        subs.push_back(BoundEventSubscriber(subTo, func, obj));
    }

    template<class Obj, class Ret, class... Args>
    void EventBoundSubscriberList::Add(Event<Args...>& subTo, Ret(Obj::*func)(Args...) const, const Obj& obj)
    {
        subs.push_back(BoundEventSubscriber(subTo, func, obj));
    }
}