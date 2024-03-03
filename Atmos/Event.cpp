
#include "Event.h"

namespace Atmos
{
    EventAnyConnection::EventAnyConnection(const EventAnyConnection &arg) : base(arg.base->Clone())
    {}

    EventAnyConnection::EventAnyConnection(EventAnyConnection &&arg) : base(std::move(arg.base))
    {}

    EventAnyConnection& EventAnyConnection::operator=(const EventAnyConnection &arg)
    {
        base.reset(arg.base->Clone());
        return *this;
    }

    EventAnyConnection& EventAnyConnection::operator=(EventAnyConnection &&arg)
    {
        base = std::move(arg.base);
        return *this;
    }

    bool EventAnyConnection::operator==(const EventAnyConnection &arg) const
    {
        if (!base || !arg.base)
            return base == arg.base;

        if (typeid(base.get()) != typeid(arg.base.get()))
            return false;

        return base->Compare(arg);
    }

    bool EventAnyConnection::operator!=(const EventAnyConnection &arg) const
    {
        return !(*this == arg);
    }

    void EventAnyConnection::Sever()
    {
        if (!base)
            return;

        base->Sever();
    }

    bool EventAnyConnection::IsValid() const
    {
        if (!base)
            return false;

        return base->IsValid();
    }

    EventScopedConnection::EventScopedConnection(const EventScopedConnection &arg) : base((arg.base) ? arg.base->Clone() : nullptr)
    {}

    EventScopedConnection::EventScopedConnection(EventScopedConnection &&arg) : base(std::move(arg.base))
    {}

    EventScopedConnection::~EventScopedConnection()
    {
        if (base)
            base->Sever();
    }

    EventScopedConnection& EventScopedConnection::operator=(const EventScopedConnection &arg)
    {
        base.reset((arg.base) ? arg.base->Clone() : nullptr);
        return *this;
    }

    EventScopedConnection& EventScopedConnection::operator=(EventScopedConnection &&arg)
    {
        base = std::move(arg.base);
        return *this;
    }

    bool EventScopedConnection::operator==(const EventScopedConnection &arg) const
    {
        return base == arg.base;
    }

    bool EventScopedConnection::operator!=(const EventScopedConnection &arg) const
    {
        return !(*this == arg);
    }

    void EventScopedConnection::Reset()
    {
        base.reset();
    }

    void EventScopedConnection::Sever()
    {
        if (base)
        {
            base->Sever();
            base.reset();
        }
    }

    bool EventScopedConnection::IsValid() const
    {
        return base.get() != nullptr && base->IsValid();
    }

    void EventScopedConnectionManager::Add(const EventScopedConnection &add)
    {
        slots.push_back(add);
    }

    void EventScopedConnectionManager::Add(EventScopedConnection &&add)
    {
        slots.push_back(std::move(add));
    }

    void EventScopedConnectionManager::Clear()
    {
        slots.clear();
    }

    void EventBoundSubscriber::Subscribe()
    {
        if (IsValid())
            base->Sub();
    }

    void EventBoundSubscriber::Unsubscribe()
    {
        if (IsValid())
            base->connection.Sever();
    }

    bool EventBoundSubscriber::IsValid() const
    {
        return base.get() != nullptr;
    }

    void EventBoundSubscriberManager::Subscribe()
    {
        for (auto& loop : subs)
            loop.Subscribe();
    }

    void EventBoundSubscriberManager::Unsubscribe()
    {
        for (auto& loop : subs)
            loop.Unsubscribe();
    }

    bool EventBoundSubscriberManager::IsValid() const
    {
        for (auto& loop : subs)
        {
            if (!loop.IsValid())
                return false;
        }

        return true;
    }
}