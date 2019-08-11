#pragma once

#include <vector>
#include <list>

#include "Serialization.h"
#include <Inscription/ListScribe.h>

namespace Atmos
{
    // Allows random access in a sequence without invalidating iterators/pointers
    template<class T>
    class RandomAccessSequence
    {
    public:
        typedef T ValueT;
    private:
        typedef std::list<ValueT> List;
    public:
        typedef typename List::size_type ID;
        typedef typename List::size_type size_type;
        typedef typename List::iterator iterator;
        typedef typename List::const_iterator const_iterator;
        typedef typename List::reverse_iterator reverse_iterator;
        typedef typename List::const_reverse_iterator const_reverse_iterator;
        typedef typename List::reference reference;
        typedef typename List::const_reference const_reference;
    private:
        typedef std::vector<iterator> Vector;
    private:
        INSCRIPTION_ACCESS;
    private:
        List list;
        Vector vector;
        void FillVector(const RandomAccessSequence &arg);
    public:
        RandomAccessSequence() = default;
        RandomAccessSequence(const RandomAccessSequence &arg);
        RandomAccessSequence(RandomAccessSequence &&arg);
        RandomAccessSequence& operator=(const RandomAccessSequence &arg);
        RandomAccessSequence& operator=(RandomAccessSequence &&arg);
        bool operator==(const RandomAccessSequence &arg) const;
        bool operator!=(const RandomAccessSequence &arg) const;
        iterator operator[](ID index);
        const_iterator operator[](ID index) const;
        iterator AddBack(const ValueT &add);
        iterator AddBack(ValueT &&add);
        iterator Add(ID index, const ValueT &add);
        iterator Add(ID index, ValueT &&add);
        iterator Remove(ID index);
        iterator Remove(iterator remove);
        iterator Remove(const_iterator remove);
        reverse_iterator Remove(reverse_iterator remove);
        reverse_iterator Remove(const_reverse_iterator remove);
        void Clear();
        iterator Find(ID index);
        const_iterator Find(ID index) const;

        iterator erase(ID remove);
        iterator erase(iterator remove);
        iterator erase(const_iterator remove);
        reverse_iterator erase(reverse_iterator remove);
        reverse_iterator erase(const_reverse_iterator remove);

        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;
        reverse_iterator rbegin();
        const_reverse_iterator rbegin() const;
        reverse_iterator rend();
        const_reverse_iterator rend() const;
        reference back();
        const_reference back() const;
        bool empty() const;
        size_type size() const;
        void clear();
    };

    template<class T>
    void RandomAccessSequence<T>::FillVector(const RandomAccessSequence &arg)
    {
        vector.clear();
        for (auto loop = list.begin(); loop != list.end(); loop++)
            vector.push_back(loop);
    }

    template<class T>
    RandomAccessSequence<T>::RandomAccessSequence(const RandomAccessSequence &arg) : list(arg.list)
    {
        FillVector(arg);
    }

    template<class T>
    RandomAccessSequence<T>::RandomAccessSequence(RandomAccessSequence &&arg) : list(std::move(arg.list))
    {
        FillVector(arg);
    }

    template<class T>
    RandomAccessSequence<T>& RandomAccessSequence<T>::operator=(const RandomAccessSequence &arg)
    {
        list = arg.list;
        FillVector(arg);
        return *this;
    }

    template<class T>
    RandomAccessSequence<T>& RandomAccessSequence<T>::operator=(RandomAccessSequence &&arg)
    {
        list = std::move(arg.list);
        FillVector(arg);
        return *this;
    }

    template<class T>
    bool RandomAccessSequence<T>::operator==(const RandomAccessSequence &arg) const
    {
        return list == arg.list;
    }

    template<class T>
    bool RandomAccessSequence<T>::operator!=(const RandomAccessSequence &arg) const
    {
        return !(*this == arg);
    }

    template<class T>
    typename RandomAccessSequence<T>::iterator RandomAccessSequence<T>::operator[](ID index)
    {
        return Find(index);
    }

    template<class T>
    typename RandomAccessSequence<T>::const_iterator RandomAccessSequence<T>::operator[](ID index) const
    {
        return Find(index);
    }

    template<class T>
    typename RandomAccessSequence<T>::iterator RandomAccessSequence<T>::AddBack(const ValueT &add)
    {
        list.push_back(add);
        vector.push_back(--list.end());
        return --list.end();
    }

    template<class T>
    typename RandomAccessSequence<T>::iterator RandomAccessSequence<T>::AddBack(ValueT &&add)
    {
        list.push_back(std::move(add));
        vector.push_back(--list.end());
        return --list.end();
    }

    template<class T>
    typename RandomAccessSequence<T>::iterator RandomAccessSequence<T>::Add(ID index, const ValueT &add)
    {
        if (index > size())
            return list.end();

        if (index == size())
            return AddBack(add);

        auto itr = --list.end();
        list.resize(index - 1);

        for (; itr != list.end(); ++itr)
            vector.push_back(itr);

        return AddBack(add);
    }

    template<class T>
    typename RandomAccessSequence<T>::iterator RandomAccessSequence<T>::Add(ID index, ValueT &&add)
    {
        if (index > size())
            return list.end();

        if (index == size())
            return AddBack(std::move(add));

        auto itr = --list.end();
        list.resize(index - 1);

        for (; itr != list.end(); ++itr)
            vector.push_back(itr);

        return AddBack(std::move(add));
    }

    template<class T>
    typename RandomAccessSequence<T>::iterator RandomAccessSequence<T>::Remove(ID index)
    {
        if (index >= size())
            return list.end();

        auto found = vector.begin() + index;
        auto ret = list.erase(*found);
        vector.erase(found);
        return ret;
    }

    template<class T>
    typename RandomAccessSequence<T>::iterator RandomAccessSequence<T>::Remove(iterator remove)
    {
        for (auto loop = vector.begin(); loop != vector.end(); loop++)
        {
            if (*loop == remove)
            {
                vector.erase(loop);
                return list.erase(remove);
            }
        }

        return list.end();
    }

    template<class T>
    typename RandomAccessSequence<T>::iterator RandomAccessSequence<T>::Remove(const_iterator remove)
    {
        for (auto loop = vector.begin(); loop != vector.end(); loop++)
        {
            if (*loop == remove)
            {
                vector.erase(loop);
                return list.erase(remove);
            }
        }

        return list.end();
    }

    template<class T>
    typename RandomAccessSequence<T>::reverse_iterator RandomAccessSequence<T>::Remove(reverse_iterator remove)
    {
        for (auto loop = vector.rbegin(); loop != vector.rend(); loop++)
        {
            if (static_cast<const_reverse_iterator>(*loop) == remove)
            {
                vector.erase(std::next(loop).base());
                auto ret = list.erase(std::next(remove).base());
                ++ret;
                return reverse_iterator(ret);
            }
        }

        return list.rend();
    }

    template<class T>
    typename RandomAccessSequence<T>::reverse_iterator RandomAccessSequence<T>::Remove(const_reverse_iterator remove)
    {
        for (auto loop = vector.rbegin(); loop != vector.rend(); loop++)
        {
            if (static_cast<const_reverse_iterator>(*loop) == remove)
            {
                vector.erase(std::next(loop).base());
                auto ret = list.erase(std::next(remove).base());
                ++ret;
                return reverse_iterator(ret);
            }
        }

        return list.rend();
    }

    template<class T>
    void RandomAccessSequence<T>::Clear()
    {
        list.clear();
        vector.clear();
    }

    template<class T>
    typename RandomAccessSequence<T>::iterator RandomAccessSequence<T>::Find(ID index)
    {
        if (index >= size())
            return list.end();

        return vector[index];
    }

    template<class T>
    typename RandomAccessSequence<T>::const_iterator RandomAccessSequence<T>::Find(ID index) const
    {
        if (index >= size())
            return list.end();

        return vector[index];
    }

    template<class T>
    typename RandomAccessSequence<T>::iterator RandomAccessSequence<T>::erase(ID remove)
    {
        return Remove(remove);
    }

    template<class T>
    typename RandomAccessSequence<T>::iterator RandomAccessSequence<T>::erase(iterator remove)
    {
        return Remove(remove);
    }

    template<class T>
    typename RandomAccessSequence<T>::iterator RandomAccessSequence<T>::erase(const_iterator remove)
    {
        return Remove(remove);
    }

    template<class T>
    typename RandomAccessSequence<T>::reverse_iterator RandomAccessSequence<T>::erase(reverse_iterator remove)
    {
        return Remove(remove);
    }

    template<class T>
    typename RandomAccessSequence<T>::reverse_iterator RandomAccessSequence<T>::erase(const_reverse_iterator remove)
    {
        return Remove(remove);
    }

    template<class T>
    typename RandomAccessSequence<T>::iterator RandomAccessSequence<T>::begin()
    {
        return list.begin();
    }

    template<class T>
    typename RandomAccessSequence<T>::const_iterator RandomAccessSequence<T>::begin() const
    {
        return list.begin();
    }

    template<class T>
    typename RandomAccessSequence<T>::iterator RandomAccessSequence<T>::end()
    {
        return list.end();
    }

    template<class T>
    typename RandomAccessSequence<T>::const_iterator RandomAccessSequence<T>::end() const
    {
        return list.end();
    }

    template<class T>
    typename RandomAccessSequence<T>::reverse_iterator RandomAccessSequence<T>::rbegin()
    {
        return list.rbegin();
    }

    template<class T>
    typename RandomAccessSequence<T>::const_reverse_iterator RandomAccessSequence<T>::rbegin() const
    {
        return list.rbegin();
    }

    template<class T>
    typename RandomAccessSequence<T>::reverse_iterator RandomAccessSequence<T>::rend()
    {
        return list.rend();
    }

    template<class T>
    typename RandomAccessSequence<T>::const_reverse_iterator RandomAccessSequence<T>::rend() const
    {
        return list.rend();
    }

    template<class T>
    typename RandomAccessSequence<T>::reference RandomAccessSequence<T>::back()
    {
        return list.back();
    }

    template<class T>
    typename RandomAccessSequence<T>::const_reference RandomAccessSequence<T>::back() const
    {
        return list.back();
    }

    template<class T>
    bool RandomAccessSequence<T>::empty() const
    {
        return list.empty();
    }

    template<class T>
    typename RandomAccessSequence<T>::size_type RandomAccessSequence<T>::size() const
    {
        return list.size();
    }

    template<class T>
    void RandomAccessSequence<T>::clear()
    {
        Clear();
    }
}

namespace Inscription
{
    template<class T>
    class Scribe<::Atmos::RandomAccessSequence<T>, BinaryArchive> :
        public CompositeScribe<::Atmos::RandomAccessSequence<T>, BinaryArchive>
    {
    private:
        using BaseT = CompositeScribe<::Atmos::RandomAccessSequence<T>, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;

        using BaseT::Scriven;
        using BaseT::Construct;
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override;
    };

    template<class T>
    void Scribe<::Atmos::RandomAccessSequence<T>, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        archive(object.list);
        if (archive.IsInput())
            object.FillVector(object);
    }

    template<class T>
    void Scribe<::Atmos::RandomAccessSequence<T>, BinaryArchive>::ConstructImplementation(
        ObjectT* storage, ArchiveT& archive)
    {
        DoBasicConstruction(storage, archive);
    }
}