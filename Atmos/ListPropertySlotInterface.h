#pragma once

#include "PropertyCore.h"

namespace Atmos
{
    template<class T>
    class Property;

    template<class T>
    class ListPropertySlotInterface : public virtual PropertyCore<T>
    {
    private:
        typedef typename std::remove_reference<T>::type UnderlyingT;
    public:
        typedef typename UnderlyingT::iterator iterator;
        typedef typename UnderlyingT::const_iterator const_iterator;
        typedef typename UnderlyingT::value_type value_type;
        typedef typename UnderlyingT::size_type size_type;
    public:
        virtual ~ListPropertySlotInterface() = 0;

        void push_back(const value_type& value);
        void push_back(value_type&& value);

        iterator at(size_type position);
        const_iterator at(size_type position) const;

        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;
    protected:
        using PropertyCore<T>::GetInternal;
        using PropertyCore<T>::SetInternal;
    };

    template<class T>
    ListPropertySlotInterface<T>::~ListPropertySlotInterface()
    {}

    template<class T>
    void ListPropertySlotInterface<T>::push_back(const value_type& value)
    {
        GetInternal().push_back(value);
    }

    template<class T>
    void ListPropertySlotInterface<T>::push_back(value_type&& value)
    {
        GetInternal().push_back(std::move(value));
    }

    template<class T>
    typename ListPropertySlotInterface<T>::iterator ListPropertySlotInterface<T>::at(size_type position)
    {
        return std::next(begin(), position);
    }

    template<class T>
    typename ListPropertySlotInterface<T>::const_iterator ListPropertySlotInterface<T>::at(size_type position) const
    {
        return std::next(begin(), position);
    }

    template<class T>
    typename ListPropertySlotInterface<T>::iterator ListPropertySlotInterface<T>::begin()
    {
        return GetInternal().begin();
    }

    template<class T>
    typename ListPropertySlotInterface<T>::iterator ListPropertySlotInterface<T>::end()
    {
        return GetInternal().end();
    }

    template<class T>
    typename ListPropertySlotInterface<T>::const_iterator ListPropertySlotInterface<T>::begin() const
    {
        return GetInternal().begin();
    }

    template<class T>
    typename ListPropertySlotInterface<T>::const_iterator ListPropertySlotInterface<T>::end() const
    {
        return GetInternal().end();
    }
}