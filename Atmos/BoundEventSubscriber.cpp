
#include "BoundEventSubscriber.h"

namespace Atmos
{
    void BoundEventSubscriber::Subscribe()
    {
        if (IsValid())
            base->Sub();
    }

    void BoundEventSubscriber::Unsubscribe()
    {
        if (IsValid())
            base->connection.Sever();
    }

    bool BoundEventSubscriber::IsValid() const
    {
        return base.get() != nullptr;
    }

    BoundEventSubscriber::Base::~Base()
    {}
}