#pragma once

#include <vector>

#include "Serialization.h"
#include <Inscription/ContainerSize.h>

namespace Atmos
{
    template<class T>
    class IntervalList
    {
    public:
        typedef T ValueT;
        typedef size_t SizeT;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        static constexpr ValueT lowest = std::numeric_limits<T>::lowest();

        class Node
        {
        public:
            typedef typename IntervalList::ValueT ValueT;
        private:
            friend IntervalList;
        private:
            ValueT start;
            ValueT size;
        public:
            Node(ValueT start = lowest, ValueT size = lowest);
            bool operator==(const Node &arg) const;
            bool operator!=(const Node &arg) const;
            ValueT GetStart() const;
            ValueT GetEnd() const;
            ValueT GetSize() const;
            bool IsIncluded(ValueT val) const;
        };

        friend Node;
    private:
        typedef std::vector<Node> NodeVector;
        NodeVector vector;

        static ValueT GetEndValue(ValueT start, ValueT size);
    public:
        typedef typename NodeVector::const_iterator const_iterator;
        typedef typename NodeVector::const_reverse_iterator const_reverse_iterator;
    public:
        IntervalList() = default;
        IntervalList(const IntervalList &arg) = default;
        IntervalList(IntervalList &&arg);
        IntervalList& operator=(const IntervalList &arg) = default;
        IntervalList& operator=(IntervalList &&arg);

        bool operator==(const IntervalList &arg) const;
        bool operator!=(const IntervalList &arg) const;

        void Include(ValueT val);
        void Remove(ValueT val);
        bool IsIncluded(ValueT val) const;

        void Clear();

        SizeT Size() const;

        bool IsEmpty() const;

        const_iterator begin() const;
        const_iterator end() const;
        const_reverse_iterator rbegin() const;
        const_reverse_iterator rend() const;
    };

    template<class T>
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(IntervalList<T>)
    {
        ::Inscription::TrackingChangerStack tracking(scribe, false);
        if (scribe.IsOutput())
        {
            ::Inscription::ContainerSize size(vector.size());
            scribe.Save(size);

            for (auto& loop : vector)
            {
                scribe.Save(loop.GetStart());
                scribe.Save(loop.GetSize());
            }
        }
        else // INPUT
        {
            ::Inscription::ContainerSize size;
            scribe.Load(size);

            while (size-- > 0)
            {
                ValueT start;
                scribe.Load(start);

                ValueT size;
                scribe.Load(size);

                vector.push_back(Node(start, size));
            }
        }
    }

    template<class T>
    IntervalList<T>::Node::Node(ValueT start, ValueT size) : start(start), size(size)
    {}

    template<class T>
    bool IntervalList<T>::Node::operator==(const Node &arg) const
    {
        return start == arg.start && size == arg.size;
    }

    template<class T>
    bool IntervalList<T>::Node::operator!=(const Node &arg) const
    {
        return !(*this == arg);
    }

    template<class T>
    typename IntervalList<T>::Node::ValueT IntervalList<T>::Node::GetStart() const
    {
        return start;
    }

    template<class T>
    typename IntervalList<T>::Node::ValueT IntervalList<T>::Node::GetEnd() const
    {
        return IntervalList::GetEndValue(start, size);
    }

    template<class T>
    typename IntervalList<T>::Node::ValueT IntervalList<T>::Node::GetSize() const
    {
        return size;
    }

    template<class T>
    bool IntervalList<T>::Node::IsIncluded(ValueT val) const
    {
        return start <= val && GetEnd() >= val;
    }

    template<class T>
    IntervalList<T>::IntervalList(IntervalList &&arg) : vector(std::move(arg.vector))
    {}

    template<class T>
    IntervalList<T>& IntervalList<T>::operator=(IntervalList &&arg)
    {
        vector = std::move(arg.vector);
        return *this;
    }

    template<class T>
    typename IntervalList<T>::ValueT IntervalList<T>::GetEndValue(ValueT start, ValueT size)
    {
        return start + size - 1;
    }

    template<class T>
    bool IntervalList<T>::operator==(const IntervalList &arg) const
    {
        return vector == arg.vector;
    }

    template<class T>
    bool IntervalList<T>::operator!=(const IntervalList &arg) const
    {
        return !(*this == arg);
    }

    template<class T>
    void IntervalList<T>::Include(ValueT val)
    {
        if (IsIncluded(val))
            return;

        // If the vector is empty
        if (vector.empty())
        {
            vector.push_back(Node(val, 1));
            return;
        }

        // Check if we need to make an interval right at the beginning of the list
        if (vector.begin()->start - 1 > val)
        {
            vector.insert(vector.begin(), Node(val, 1));
            return;
        }

        // Check if we need to make an interval at the end of the list
        if (std::prev(vector.end())->GetEnd() + 1 < val)
        {
            vector.push_back(Node(val, 1));
            return;
        }

        auto prev = vector.begin();
        auto next = std::next(vector.begin());

        for (auto loop = vector.begin(); loop != vector.end();)
        {
            // Check the value immediately before the start of this node
            if (val != lowest && loop->start - 1 == val)
            {
                // Check if we need to concatenate two intervals
                if (prev->GetEnd() + 1 == val)
                {
                    prev->size += loop->size + 1;
                    vector.erase(loop);
                    return;
                }

                // We don't need to concatenate
                --loop->start;
                ++loop->size;
                return;
            }

            // Check the value immediately after the end of this node
            if (loop->GetEnd() + 1 == val)
            {
                // Check if we need to concatenate two intervals
                if (next != vector.end() && next->start - 1 == val)
                {
                    loop->size += next->size + 1;
                    vector.erase(next);
                    return;
                }

                // We don't need to concatenate
                ++loop->size;
                return;
            }

            // Check if we need to make a new interval between this one and the next one
            if (next != vector.end() && next->start < val && loop->GetEnd() > val)
            {
                vector.insert(loop, Node(val, 1));
                return;
            }

            ++prev;
            ++next;
            ++loop;
        }
    }

    template<class T>
    void IntervalList<T>::Remove(ValueT val)
    {
        // If the vector is empty
        if (vector.empty())
            return;

        for (auto loop = vector.begin(); loop != vector.end(); ++loop)
        {
            // Check the value at the start of the node
            if (loop->start == val)
            {
                ++loop->start;
                --loop->size;
                if (loop->size == 0)
                    vector.erase(loop);
                return;
            }

            const ValueT endValue = loop->GetEnd();
            // Check the value at the end of the node
            if (endValue == val)
            {
                --loop->size;
                if (loop->size == 0)
                    vector.erase(loop);
                return;
            }

            // Check if we need to split this node (deleting something in the middle)
            if (loop->start < val && endValue > val)
            {
                ValueT oSize = loop->size;
                loop->size = loop->size - (endValue - val + 1);
                vector.insert(std::next(loop), Node(GetEndValue(loop->start, loop->size) + 2, oSize - loop->size - 1));
                return;
            }
        }
    }

    template<class T>
    bool IntervalList<T>::IsIncluded(ValueT val) const
    {
        for (auto& loop : vector)
        {
            if (loop.IsIncluded(val))
                return true;
        }

        return false;
    }

    template<class T>
    void IntervalList<T>::Clear()
    {
        vector.clear();
    }

    template<class T>
    typename IntervalList<T>::SizeT IntervalList<T>::Size() const
    {
        SizeT ret = 0;
        for (auto& loop : vector)
            ret += loop.GetSize();
        return ret;
    }

    template<class T>
    bool IntervalList<T>::IsEmpty() const
    {
        return vector.empty();
    }

    template<class T>
    typename IntervalList<T>::const_iterator IntervalList<T>::begin() const
    {
        return vector.begin();
    }

    template<class T>
    typename IntervalList<T>::const_iterator IntervalList<T>::end() const
    {
        return vector.end();
    }

    template<class T>
    typename IntervalList<T>::const_reverse_iterator IntervalList<T>::rbegin() const
    {
        return vector.rbegin();
    }

    template<class T>
    typename IntervalList<T>::const_reverse_iterator IntervalList<T>::rend() const
    {
        return vector.rend();
    }
}