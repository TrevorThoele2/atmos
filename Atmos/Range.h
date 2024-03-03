#pragma once

namespace Atmos
{
    template<class T>
    struct Range
    {
        T min = 0;
        T max = 0;

        Range() = default;
        Range(T min, T max);

        bool operator==(const Range& arg) const;
        bool operator!=(const Range& arg) const;

        T Distance() const;
    };

    template<class T>
    Range<T>::Range(T min, T max) : min(min), max(max)
    {}

    template<class T>
    bool Range<T>::operator==(const Range& arg) const
    {
        return min == arg.min && max == arg.max;
    }

    template<class T>
    bool Range<T>::operator!=(const Range& arg) const
    {
        return min == arg.min && max == arg.max;
    }

    template<class T>
    T Range<T>::Distance() const
    {
        return max - min;
    }
}