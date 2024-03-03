#pragma once

#include "ScopedEventConnection.h"

namespace Atmos
{
    class ScopedEventConnectionList
    {
    public:
        template<class... Args>
        void Add(const ::Chroma::EventConnection<Args...>& connection);
        template<class... Args>
        void Add(::Chroma::EventConnection<Args...>&& connection);
        void Add(const ScopedEventConnection& add);
        void Add(ScopedEventConnection&& add);

        void Clear();
    private:
        std::list<ScopedEventConnection> slots;
    };

    template<class... Args>
    void ScopedEventConnectionList::Add(const ::Chroma::EventConnection<Args...>& connection)
    {
        slots.push_back(ScopedEventConnection(connection));
    }

    template<class... Args>
    void ScopedEventConnectionList::Add(::Chroma::EventConnection<Args...>&& connection)
    {
        slots.push_back(ScopedEventConnection(std::move(connection)));
    }
}