#pragma once

#include "Serialization.h"

namespace Atmos
{
    template<class T>
    class Range
    {
    public:
        typedef T Value;
    public:
        Value begin, end;

        Range() = default;
        Range(const Value& begin, const Value& end);
        bool operator==(const Range& arg) const;
        bool operator!=(const Range& arg) const;

        void SetBegin(const Value& set);
        void SetEnd(const Value& set);
        const Value& GetBegin() const;
        const Value& GetEnd() const;
    private:
        INSCRIPTION_ACCESS;
    };

    template<class T>
    Range<T>::Range(const Value& begin, const Value& end) : begin(begin), end(end)
    {}

    template<class T>
    bool Range<T>::operator==(const Range<T>& arg) const
    {
        return begin == arg.begin && end == arg.end;
    }

    template<class T>
    bool Range<T>::operator!=(const Range& arg) const
    {
        return !(*this == arg);
    }

    template<class T>
    void Range<T>::SetBegin(const Value& setTo)
    {
        if (begin <= setTo)
            begin = setTo;
    }

    template<class T>
    void Range<T>::SetEnd(const Value& setTo)
    {
        if (setTo >= begin)
            end = setTo;
    }

    template<class T>
    typename const Range<T>::Value& Range<T>::GetBegin() const
    {
        return begin;
    }

    template<class T>
    typename const Range<T>::Value& Range<T>::GetEnd() const
    {
        return end;
    }
}

namespace Inscription
{
    template<class T>
    class Scribe<::Atmos::Range<T>, BinaryArchive> : public CompositeScribe<::Atmos::Range<T>, BinaryArchive>
    {
    private:
        using BaseT = typename CompositeScribe<::Atmos::Range<T>, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive)
        {
            archive(object.begin);
            archive(object.end);
        }
    };
}