
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
        typedef std::vector<T> Container;
    public:
        typedef typename Container::iterator iterator;
        typedef typename Container::const_iterator const_iterator;
        typedef typename Container::reference reference;
        typedef typename Container::const_reference const_reference;
        typedef typename Container::size_type size_type;
    private:
        Container container;
        Compare comp;
    public:
        void Push(const T &push);
        void Push(T &&push);
        void Pop();
        reference Top();
        const_reference Top() const;
        void Heapify();

        void Erase(iterator erase);

        iterator Find(const T &find);
        const_iterator Find(const T &find) const;

        bool IsEmpty() const;
        size_type Size() const;

        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;
    };

    template<class T, class Compare>
    void BinaryHeap<T, Compare>::Push(const T &push)
    {
        container.push_back(push);
        std::push_heap(container.begin(), container.end(), comp);
    }

    template<class T, class Compare>
    void BinaryHeap<T, Compare>::Push(T &&push)
    {
        container.push_back(std::move(push));
        std::push_heap(container.begin(), container.end(), comp);
    }

    template<class T, class Compare>
    void BinaryHeap<T, Compare>::Pop()
    {
        std::pop_heap(container.begin(), container.end(), comp);
        container.pop_back();
    }

    template<class T, class Compare>
    typename BinaryHeap<T, Compare>::reference BinaryHeap<T, Compare>::Top()
    {
        return container.front();
    }

    template<class T, class Compare>
    typename BinaryHeap<T, Compare>::const_reference BinaryHeap<T, Compare>::Top() const
    {
        return container.front();
    }

    template<class T, class Compare>
    void BinaryHeap<T, Compare>::Heapify()
    {
        std::make_heap(container.begin(), container.end(), comp);
    }

    template<class T, class Compare>
    void BinaryHeap<T, Compare>::Erase(iterator erase)
    {
        container.erase(erase);
        Heapify();
    }

    template<class T, class Compare>
    typename BinaryHeap<T, Compare>::iterator BinaryHeap<T, Compare>::Find(const T &find)
    {
        for (auto loop = container.begin(); loop != container.end(); loop++)
        {
            if (*loop == find)
                return loop;
        }

        return container.end();
    }

    template<class T, class Compare>
    typename BinaryHeap<T, Compare>::const_iterator BinaryHeap<T, Compare>::Find(const T &find) const
    {
        for (auto loop = container.begin(); loop != container.end(); loop++)
        {
            if (*loop == find)
                return loop;
        }

        return container.end();
    }

    template<class T, class Compare>
    bool BinaryHeap<T, Compare>::IsEmpty() const
    {
        return container.empty();
    }

    template<class T, class Compare>
    typename BinaryHeap<T, Compare>::size_type BinaryHeap<T, Compare>::Size() const
    {
        return container.size();
    }

    template<class T, class Compare>
    typename BinaryHeap<T, Compare>::iterator BinaryHeap<T, Compare>::begin()
    {
        return container.begin();
    }

    template<class T, class Compare>
    typename BinaryHeap<T, Compare>::iterator BinaryHeap<T, Compare>::end()
    {
        return container.end();
    }

    template<class T, class Compare>
    typename BinaryHeap<T, Compare>::const_iterator BinaryHeap<T, Compare>::begin() const
    {
        return container.begin();
    }

    template<class T, class Compare>
    typename BinaryHeap<T, Compare>::const_iterator BinaryHeap<T, Compare>::end() const
    {
        return container.end();
    }
}