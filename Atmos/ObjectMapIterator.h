#pragma once

namespace Atmos
{
    template<class ReturnT, class WrapT>
    class ObjectMapIteratorBase
    {
    public:
        ObjectMapIteratorBase(const WrapT& wrap);
        ObjectMapIteratorBase(const ObjectMapIteratorBase& arg);

        ObjectMapIteratorBase& operator=(const ObjectMapIteratorBase& arg);

        bool operator==(const ObjectMapIteratorBase& arg) const;
        bool operator!=(const ObjectMapIteratorBase& arg) const;

        ReturnT& operator*();
        ReturnT* operator->();

        ObjectMapIteratorBase& operator++();
        ObjectMapIteratorBase operator++(int) const;
        ObjectMapIteratorBase& operator--();
        ObjectMapIteratorBase operator--(int) const;

        ObjectID ID() const;
    private:
        WrapT wrapped;
    private:
        template<class U>
        friend class ObjectMap;
    };

    template<class ReturnT, class WrapT>
    ObjectMapIteratorBase<ReturnT, WrapT>::ObjectMapIteratorBase(const WrapT& wrap) : wrapped(wrap)
    {}

    template<class ReturnT, class WrapT>
    ObjectMapIteratorBase<ReturnT, WrapT>::ObjectMapIteratorBase(const ObjectMapIteratorBase& arg) : wrapped(arg.wrapped)
    {}

    template<class ReturnT, class WrapT>
    ObjectMapIteratorBase<ReturnT, WrapT>& ObjectMapIteratorBase<ReturnT, WrapT>::operator=(const ObjectMapIteratorBase& arg)
    {
        wrapped = arg.wrapped;
        return *this;
    }

    template<class ReturnT, class WrapT>
    bool ObjectMapIteratorBase<ReturnT, WrapT>::operator==(const ObjectMapIteratorBase& arg) const
    {
        return wrapped == arg.wrapped;
    }

    template<class ReturnT, class WrapT>
    bool ObjectMapIteratorBase<ReturnT, WrapT>::operator!=(const ObjectMapIteratorBase& arg) const
    {
        return !(*this == arg);
    }

    template<class ReturnT, class WrapT>
    ReturnT& ObjectMapIteratorBase<ReturnT, WrapT>::operator*()
    {
        return wrapped->second;
    }

    template<class ReturnT, class WrapT>
    ReturnT* ObjectMapIteratorBase<ReturnT, WrapT>::operator->()
    {
        return &wrapped->second;
    }

    template<class ReturnT, class WrapT>
    ObjectMapIteratorBase<ReturnT, WrapT>& ObjectMapIteratorBase<ReturnT, WrapT>::operator++()
    {
        ++wrapped;
        return *this;
    }

    template<class ReturnT, class WrapT>
    ObjectMapIteratorBase<ReturnT, WrapT> ObjectMapIteratorBase<ReturnT, WrapT>::operator++(int) const
    {
        auto copy = *this;
        ++wrapped;
        return copy;
    }

    template<class ReturnT, class WrapT>
    ObjectMapIteratorBase<ReturnT, WrapT>& ObjectMapIteratorBase<ReturnT, WrapT>::operator--()
    {
        --wrapped;
        return *this;
    }

    template<class ReturnT, class WrapT>
    ObjectMapIteratorBase<ReturnT, WrapT> ObjectMapIteratorBase<ReturnT, WrapT>::operator--(int) const
    {
        auto copy = *this;
        --wrapped;
        return copy;
    }

    template<class ReturnT, class WrapT>
    ObjectID ObjectMapIteratorBase<ReturnT, WrapT>::ID() const
    {
        return wrapped.first;
    }
}