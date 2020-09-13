
#pragma once

#include <vector>
#include <algorithm>
#include <functional>

namespace Atmos
{
    template<class T, class Compare = std::less<T>>
    class BinaryHeap
    {
    private:
        using Underlying = std::vector<T>;
    public:
        using iterator = typename Underlying::iterator;
        using const_iterator = typename Underlying::const_iterator;
        using reference = typename Underlying::reference;
        using const_reference = typename Underlying::const_reference;
        using size_type = typename Underlying::size_type;
    public:
        void Push(const T& push);
        void Push(T&& push);
        void Pop();
        void Heapify();

        void Erase(iterator erase);

        [[nodiscard]] reference Top();
        [[nodiscard]] const_reference Top() const;

        [[nodiscard]] iterator Find(const T &find);
        [[nodiscard]] const_iterator Find(const T &find) const;

        [[nodiscard]] bool IsEmpty() const;
        [[nodiscard]] size_type Size() const;

        [[nodiscard]] iterator begin();
        [[nodiscard]] iterator end();
        [[nodiscard]] const_iterator begin() const;
        [[nodiscard]] const_iterator end() const;
    private:
        Underlying underlying;
        Compare compare;
    };

    template<class T, class Compare>
    void BinaryHeap<T, Compare>::Push(const T& push)
    {
        underlying.push_back(push);
        std::push_heap(underlying.begin(), underlying.end(), compare);
    }

    template<class T, class Compare>
    void BinaryHeap<T, Compare>::Push(T&& push)
    {
        underlying.push_back(std::move(push));
        std::push_heap(underlying.begin(), underlying.end(), compare);
    }

    template<class T, class Compare>
    void BinaryHeap<T, Compare>::Pop()
    {
        std::pop_heap(underlying.begin(), underlying.end(), compare);
        underlying.pop_back();
    }

    template<class T, class Compare>
    void BinaryHeap<T, Compare>::Heapify()
    {
        std::make_heap(underlying.begin(), underlying.end(), compare);
    }

    template<class T, class Compare>
    void BinaryHeap<T, Compare>::Erase(iterator erase)
    {
        underlying.erase(erase);
        Heapify();
    }

    template<class T, class Compare>
    typename BinaryHeap<T, Compare>::reference BinaryHeap<T, Compare>::Top()
    {
        return underlying.front();
    }

    template<class T, class Compare>
    typename BinaryHeap<T, Compare>::const_reference BinaryHeap<T, Compare>::Top() const
    {
        return underlying.front();
    }

    template<class T, class Compare>
    typename BinaryHeap<T, Compare>::iterator BinaryHeap<T, Compare>::Find(const T &find)
    {
        for (auto entry = underlying.begin(); entry != underlying.end(); ++entry)
            if (*entry == find)
                return entry;

        return underlying.end();
    }

    template<class T, class Compare>
    typename BinaryHeap<T, Compare>::const_iterator BinaryHeap<T, Compare>::Find(const T &find) const
    {
        for (auto entry = underlying.begin(); entry != underlying.end(); ++entry)
            if (*entry == find)
                return entry;

        return underlying.end();
    }

    template<class T, class Compare>
    bool BinaryHeap<T, Compare>::IsEmpty() const
    {
        return underlying.empty();
    }

    template<class T, class Compare>
    typename BinaryHeap<T, Compare>::size_type BinaryHeap<T, Compare>::Size() const
    {
        return underlying.size();
    }

    template<class T, class Compare>
    typename BinaryHeap<T, Compare>::iterator BinaryHeap<T, Compare>::begin()
    {
        return underlying.begin();
    }

    template<class T, class Compare>
    typename BinaryHeap<T, Compare>::iterator BinaryHeap<T, Compare>::end()
    {
        return underlying.end();
    }

    template<class T, class Compare>
    typename BinaryHeap<T, Compare>::const_iterator BinaryHeap<T, Compare>::begin() const
    {
        return underlying.begin();
    }

    template<class T, class Compare>
    typename BinaryHeap<T, Compare>::const_iterator BinaryHeap<T, Compare>::end() const
    {
        return underlying.end();
    }
}