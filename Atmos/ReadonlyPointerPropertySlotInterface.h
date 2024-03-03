#pragma once

#include <type_traits>
#include "PropertyCore.h"

namespace Atmos
{
    template<class T>
    class ReadonlyPointerPropertySlotInterface : public virtual PropertyCore<T>
    {
    private:
        typedef typename std::decay<decltype(*T())>::type DereferenceT;
    public:
        virtual ~ReadonlyPointerPropertySlotInterface() = 0;

        operator bool() const
        {
            return static_cast<bool>(this->GetInternal());
        }

        const DereferenceT& operator*() const;
        const DereferenceT* operator->() const;
    protected:
        using PropertyCore<T>::GetInternal;
        using PropertyCore<T>::SetInternal;
    };

    template<class T>
    ReadonlyPointerPropertySlotInterface<T>::~ReadonlyPointerPropertySlotInterface()
    {}

    template<class T>
    typename const ReadonlyPointerPropertySlotInterface<T>::DereferenceT& ReadonlyPointerPropertySlotInterface<T>::operator*() const
    {
        return *GetInternal();
    }

    template<class T>
    typename const ReadonlyPointerPropertySlotInterface<T>::DereferenceT* ReadonlyPointerPropertySlotInterface<T>::operator->() const
    {
        return &*GetInternal();
    }
}