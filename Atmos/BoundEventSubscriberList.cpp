
#include "BoundEventSubscriberList.h"

namespace Atmos
{
    void EventBoundSubscriberList::Subscribe()
    {
        for (auto& loop : subs)
            loop.Subscribe();
    }

    void EventBoundSubscriberList::Unsubscribe()
    {
        for (auto& loop : subs)
            loop.Unsubscribe();
    }

    bool EventBoundSubscriberList::IsValid() const
    {
        for (auto& loop : subs)
        {
            if (!loop.IsValid())
                return false;
        }

        return true;
    }
}