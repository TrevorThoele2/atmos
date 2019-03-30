#pragma once

#include "ObjectTypeGraphNode.h"

namespace Atmos
{
    template<class T>
    class ObjectTypeGraphIteratorBase
    {
    public:
        ObjectTypeGraphIteratorBase(T* wrap);
        ObjectTypeGraphIteratorBase(const ObjectTypeGraphIteratorBase& arg);

        ObjectTypeGraphIteratorBase& operator=(const ObjectTypeGraphIteratorBase& arg);

        bool operator==(const ObjectTypeGraphIteratorBase& arg) const;
        bool operator!=(const ObjectTypeGraphIteratorBase& arg) const;

        T& operator*();
        T* operator->();

        ObjectTypeGraphIteratorBase& operator++();
        ObjectTypeGraphIteratorBase operator++(int) const;
    private:
        T* wrapped;
    private:
        T* Next() const;
    };

    template<class T>
    ObjectTypeGraphIteratorBase<T>::ObjectTypeGraphIteratorBase(T* wrap) : wrapped(wrap)
    {}

    template<class T>
    ObjectTypeGraphIteratorBase<T>::ObjectTypeGraphIteratorBase(const ObjectTypeGraphIteratorBase& arg) : wrapped(arg.wrapped)
    {}

    template<class T>
    ObjectTypeGraphIteratorBase<T>& ObjectTypeGraphIteratorBase<T>::operator=(const ObjectTypeGraphIteratorBase& arg)
    {
        wrapped = arg.wrapped;
        return *this;
    }

    template<class T>
    bool ObjectTypeGraphIteratorBase<T>::operator==(const ObjectTypeGraphIteratorBase& arg) const
    {
        return wrapped == arg.wrapped;
    }

    template<class T>
    bool ObjectTypeGraphIteratorBase<T>::operator!=(const ObjectTypeGraphIteratorBase& arg) const
    {
        return !(*this == arg);
    }

    template<class T>
    T& ObjectTypeGraphIteratorBase<T>::operator*()
    {
        return wrapped->second;
    }

    template<class T>
    T* ObjectTypeGraphIteratorBase<T>::operator->()
    {
        return &wrapped->second;
    }

    template<class T>
    ObjectTypeGraphIteratorBase<T>& ObjectTypeGraphIteratorBase<T>::operator++()
    {
        wrapped = Next();
        return *this;
    }

    template<class T>
    ObjectTypeGraphIteratorBase<T> ObjectTypeGraphIteratorBase<T>::operator++(int) const
    {
        auto copy = *this;
        wrapped = Next();
        return copy;
    }

    template<class T>
    T* ObjectTypeGraphIteratorBase<T>::Next() const
    {
        auto parent = wrapped->parent;
        if (!parent)
            return nullptr;

        auto found = std::find_if(parent->children.begin(), parent->children.end(), [this](const ObjectTypeGraphNode::NodePtr& arg) { return arg.get() == wrapped; });
        auto next = std::next(found);
        if (next == parent->children.end())
            return parent;

        auto child = (*next)->children.begin();
        while (child != (*next)->children.end())
        {
            next = child;
            child = (*next)->children.begin();
        }

        return next->get();
    }
}