#pragma once

#include <type_traits>
#include "PropertyCore.h"

namespace Atmos
{
    template<class T>
    class PointerPropertySlotInterface : public virtual PropertyCore<T>
    {
    private:
        typedef typename std::remove_reference<decltype(*T())>::type DereferenceT;
    public:
        virtual ~PointerPropertySlotInterface() = 0;

        operator bool() const;

        DereferenceT& operator*();
        const DereferenceT& operator*() const;
        DereferenceT* operator->();
        const DereferenceT* operator->() const;
    protected:
        using PropertyCore<T>::GetInternal;
        using PropertyCore<T>::SetInternal;
    };

    template<class T>
    PointerPropertySlotInterface<T>::~PointerPropertySlotInterface()
    {}

    template<class T>
    PointerPropertySlotInterface<T>::operator bool() const
    {
        return static_cast<bool>(GetInternal());
    }

    template<class T>
    typename PointerPropertySlotInterface<T>::DereferenceT& PointerPropertySlotInterface<T>::operator*()
    {
        return *GetInternal();
    }

    template<class T>
    typename const PointerPropertySlotInterface<T>::DereferenceT& PointerPropertySlotInterface<T>::operator*() const
    {
        return *GetInternal();
    }

    template<class T>
    typename PointerPropertySlotInterface<T>::DereferenceT* PointerPropertySlotInterface<T>::operator->()
    {
        return &*GetInternal();
    }

    template<class T>
    typename const PointerPropertySlotInterface<T>::DereferenceT* PointerPropertySlotInterface<T>::operator->() const
    {
        return &*GetInternal();
    }
}