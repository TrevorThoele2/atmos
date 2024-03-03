#pragma once

namespace Atmos
{
    template<class T>
    struct Delta
    {
        T current;
        T previous;

        bool operator==(const Delta& arg) const;
        bool operator!=(const Delta& arg) const;
    };

    template<class T>
    bool Delta<T>::operator==(const Delta& arg) const
    {
        return current == arg.current && previous == arg.previous;
    }

    template<class T>
    bool Delta<T>::operator!=(const Delta& arg) const
    {
        return !(*this == arg);
    }
}