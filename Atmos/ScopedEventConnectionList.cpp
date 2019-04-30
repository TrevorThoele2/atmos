
#include "ScopedEventConnectionList.h"

namespace Atmos
{
    void ScopedEventConnectionList::Add(const ScopedEventConnection& add)
    {
        slots.push_back(add);
    }

    void ScopedEventConnectionList::Add(ScopedEventConnection&& add)
    {
        slots.push_back(std::move(add));
    }

    void ScopedEventConnectionList::Clear()
    {
        slots.clear();
    }
}