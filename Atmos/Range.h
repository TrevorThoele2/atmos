
#pragma once

#include "Serialization.h"

namespace Atmos
{
    template<class T>
    class Range
    {
    public:
        typedef T ValueT;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    public:
        ValueT begin, end;

        Range() = default;
        Range(const ValueT &begin, const ValueT &end);
        bool operator==(const Range &arg) const;
        bool operator!=(const Range &arg) const;

        void SetBegin(const ValueT &set);
        void SetEnd(const ValueT &set);
        const ValueT& GetBegin() const;
        const ValueT& GetEnd() const;
    };

    template<class T>
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Range<T>)
    {
        scribe(begin);
        scribe(end);
    }

    template<class T>
    Range<T>::Range(const ValueT &begin, const ValueT &end) : begin(begin), end(end)
    {}

    template<class T>
    bool Range<T>::operator==(const Range<T> &arg) const
    {
        return begin == arg.begin && end == arg.end;
    }

    template<class T>
    bool Range<T>::operator!=(const Range &arg) const
    {
        return !(*this == arg);
    }

    template<class T>
    void Range<T>::SetBegin(const ValueT &setTo)
    {
        if (begin <= setTo)
            begin = setTo;
    }

    template<class T>
    void Range<T>::SetEnd(const ValueT &setTo)
    {
        if (setTo >= begin)
            end = setTo;
    }

    template<class T>
    typename const Range<T>::ValueT& Range<T>::GetBegin() const
    {
        return begin;
    }

    template<class T>
    typename const Range<T>::ValueT& Range<T>::GetEnd() const
    {
        return end;
    }
}