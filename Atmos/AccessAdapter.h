#pragma once

#include <type_traits>

namespace Atmos
{
    // Removes the ability to manipulate the count of elements in the container
    template<class Cont>
    class AccessAdapter
    {
    public:
        typedef Cont UnderlyingT;
        typedef typename UnderlyingT::iterator iterator;
        typedef typename UnderlyingT::const_iterator const_iterator;
        typedef typename UnderlyingT::size_type size_type;
    protected:
        UnderlyingT &wrapped;
    public:
        AccessAdapter(UnderlyingT &wrapped) : wrapped(wrapped) {}
        bool operator==(const AccessAdapter &arg) const { return wrapped == arg.wrapped; }
        bool operator!=(const AccessAdapter &arg) const { return !(*this == arg); }

        iterator begin() { return wrapped.begin(); }
        const_iterator begin() const { return wrapped.begin(); }
        iterator end() { return wrapped.end(); }
        const_iterator end() const { return wrapped.end(); }

        bool empty() const { return wrapped.empty(); }
        size_type size() const { return wrapped.size(); }
    };

    // Removes the ability to manipulate the count of elements in the container
    template<class Cont>
    class ConstAccessAdapter
    {
    public:
        typedef Cont UnderlyingT;
        typedef typename UnderlyingT::const_iterator const_iterator;
        typedef typename UnderlyingT::size_type size_type;
    protected:
        const UnderlyingT &wrapped;
    public:
        ConstAccessAdapter(const UnderlyingT &wrapped) : wrapped(wrapped) {}
        bool operator==(const ConstAccessAdapter &arg) const { return wrapped == arg.wrapped; }
        bool operator!=(const ConstAccessAdapter &arg) const { return !(*this == arg); }

        const_iterator begin() { return wrapped.begin(); }
        const_iterator end() { return wrapped.end(); }

        bool empty() const { return wrapped.empty(); }
        size_type size() const { return wrapped.size(); }
    };
}