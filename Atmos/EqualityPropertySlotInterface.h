#pragma once

#include "PropertyCore.h"

namespace Atmos
{
    template<class T>
    class Property;

    template<class T>
    class EqualityPropertySlotInterface : public virtual PropertyCore<T>
    {
    public:
        bool operator==(const EqualityPropertySlotInterface<T>& arg) const
        {
            return GetInternal() == arg.GetInternal();
        }

        bool operator==(const T& arg) const
        {
            return GetInternal() == arg;
        }

        bool operator!=(const EqualityPropertySlotInterface<T>& arg) const
        {
            return !(*this == arg);
        }

        bool operator!=(const T& arg) const
        {
            return !(*this == arg);
        }
    protected:
        using PropertyCore<T>::GetInternal;
        using PropertyCore<T>::SetInternal;
    };
}