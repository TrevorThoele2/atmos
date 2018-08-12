
#pragma once

#include <unordered_map>

#include "GridPosition.h"
#include "Direction.h"

#include "Serialization.h"
#include <Inscription\Scribe.h>
#include <Inscription\UnorderedMap.h>

namespace Atmos
{
    template<class Key, class T>
    class RangePatternBase
    {
    private:
        typedef std::unordered_map<Key, T> Container;
    public:
        typedef Key KeyT;
        typedef T ValueT;
        typedef typename Container::iterator iterator;
        typedef typename Container::const_iterator const_iterator;
        typedef typename Container::size_type size_type;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    protected:
        Container container;
    public:
        bool operator==(const RangePatternBase &arg) const;
        bool operator!=(const RangePatternBase &arg) const;

        template<class... Args>
        std::pair<iterator, bool> Add(const KeyT &key, Args && ... args);
        template<class... Args>
        std::pair<iterator, bool> Add(KeyT &&key, Args && ... args);
        bool Remove(const KeyT &key);
        iterator Remove(iterator remove);
        void Clear();

        bool IsWithin(const KeyT &pos) const;
        T* Find(const KeyT &key);
        const T* Find(const KeyT &key) const;

        size_type Size() const;
        size_type size() const;
        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;
    };

    template<class Key, class T>
    void RangePatternBase<Key, T>::Serialize(::Inscription::Scribe &scribe)
    {
        scribe(container);
    }

    template<class Key, class T>
    bool RangePatternBase<Key, T>::operator==(const RangePatternBase &arg) const
    {
        return container == arg.container;
    }

    template<class Key, class T>
    bool RangePatternBase<Key, T>::operator!=(const RangePatternBase &arg) const
    {
        return !(*this == arg);
    }

    template<class Key, class T>
    template<class... Args>
    std::pair<typename RangePatternBase<Key, T>::iterator, bool> RangePatternBase<Key, T>::Add(const KeyT &key, Args && ... args)
    {
        return container.emplace(key, T(std::forward<Args>(args)...));
    }

    template<class Key, class T>
    template<class... Args>
    std::pair<typename RangePatternBase<Key, T>::iterator, bool> RangePatternBase<Key, T>::Add(KeyT &&key, Args && ... args)
    {
        return container.emplace(std::move(key), std::forward<Args>(args)...);
    }

    template<class Key, class T>
    bool RangePatternBase<Key, T>::Remove(const KeyT &key)
    {
        return container.erase(key) != 0;
    }

    template<class Key, class T>
    typename RangePatternBase<Key, T>::iterator RangePatternBase<Key, T>::Remove(iterator remove)
    {
        return container.erase(remove);
    }

    template<class Key, class T>
    void RangePatternBase<Key, T>::Clear()
    {
        container.clear();
    }

    template<class Key, class T>
    bool RangePatternBase<Key, T>::IsWithin(const KeyT &pos) const
    {
        return Find(pos) != nullptr;
    }

    template<class Key, class T>
    T* RangePatternBase<Key, T>::Find(const KeyT &key)
    {
        auto found = container.find(key);
        if (found == container.end())
            return nullptr;

        return &found->second;
    }

    template<class Key, class T>
    const T* RangePatternBase<Key, T>::Find(const KeyT &key) const
    {
        auto found = container.find(key);
        if (found == container.end())
            return nullptr;

        return &found->second;
    }

    template<class Key, class T>
    typename RangePatternBase<Key, T>::size_type RangePatternBase<Key, T>::Size() const
    {
        return container.size();
    }

    template<class Key, class T>
    typename RangePatternBase<Key, T>::size_type RangePatternBase<Key, T>::size() const
    {
        return container.size();
    }

    template<class Key, class T>
    typename RangePatternBase<Key, T>::iterator RangePatternBase<Key, T>::begin()
    {
        return container.begin();
    }

    template<class Key, class T>
    typename RangePatternBase<Key, T>::const_iterator RangePatternBase<Key, T>::begin() const
    {
        return container.begin();
    }

    template<class Key, class T>
    typename RangePatternBase<Key, T>::iterator RangePatternBase<Key, T>::end()
    {
        return container.end();
    }

    template<class Key, class T>
    typename RangePatternBase<Key, T>::const_iterator RangePatternBase<Key, T>::end() const
    {
        return container.end();
    }

    template<class T>
    class RelativeRangePattern : public RangePatternBase<RelativeGridPosition, T>
    {
    private:
        typedef RangePatternBase<RelativeGridPosition, T> BaseT;
    public:
        typedef typename BaseT::KeyT KeyT;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        KeyT midPoint;
        Direction direction;
    public:
        RelativeRangePattern(const KeyT &midPoint = KeyT(0, 0, 0), const Direction &direction = Direction::ValueT::UP);
        bool operator==(const RelativeRangePattern &arg) const;
        bool operator!=(const RelativeRangePattern &arg) const;

        const Direction& GetDirection() const;
        void Rotate(Direction::RelativeValueT relVal);
        void Rotate(const Direction &requestedDirection);
    };

    template<class T>
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(RelativeRangePattern<T>)
    {
        ::Inscription::BaseSerialize<BaseT>(scribe, *this);
        scribe(midPoint);
        scribe(direction);
    }

    template<class T>
    RelativeRangePattern<T>::RelativeRangePattern(const KeyT &midPoint, const Direction &direction) : midPoint(midPoint), direction(direction)
    {}

    template<class T>
    bool RelativeRangePattern<T>::operator==(const RelativeRangePattern &arg) const
    {
        return BaseT::operator==(arg) && midPoint == arg.midPoint && direction == arg.direction;
    }

    template<class T>
    bool RelativeRangePattern<T>::operator!=(const RelativeRangePattern &arg) const
    {
        return !(*this == arg);
    }

    template<class T>
    const Direction& RelativeRangePattern<T>::GetDirection() const
    {
        return direction;
    }

    template<class T>
    void RelativeRangePattern<T>::Rotate(Direction::RelativeValueT relVal)
    {
        auto rotateMidPoint = [&]()
        {
            KeyT prevMidPoint(midPoint);

            // Rotate the mid point
            midPoint.y = prevMidPoint.x;
            midPoint.y = prevMidPoint.x;

            return prevMidPoint;
        };

        auto rotateLeft = [&]()
        {
            KeyT prevMidPoint(rotateMidPoint());

            auto loop = container.begin();
            while (loop != container.end())
            {
                auto prevCol = loop->first.x;

                auto col = (midPoint.x - (prevMidPoint.y - loop->first.y));
                auto row = (midPoint.y + (prevMidPoint.x - prevCol));

                container.emplace(KeyT(col, row, loop->first.z), std::move(loop->second));
                loop = container.erase(loop);
            }
        };

        auto rotateRight = [&]()
        {
            KeyT prevMidPoint(rotateMidPoint());

            auto loop = container.begin();
            while (loop != container.end())
            {
                auto prevCol = loop->first.x;

                auto col = (midPoint.x + (prevMidPoint.y - loop->first.y));
                auto row = (midPoint.y - (prevMidPoint.x - prevCol));

                container.emplace(KeyT(col, row, loop->first.z), std::move(loop->second));
                loop = container.erase(loop);
            }
        };

        auto rotate180 = [&]()
        {
            auto loop = container.begin();
            while (loop != container.end())
            {
                auto prevRow = loop->first.y;

                auto col = midPoint.x + (midPoint.x - loop->first.x);
                auto row = midPoint.y + (midPoint.y - prevRow);

                container.emplace(KeyT(col, row, loop->first.z), std::move(loop->second));
                loop = container.erase(loop);
            }
        };

        switch (relVal)
        {
        case Direction::RelativeValueT::SAME:
            return;
        case Direction::RelativeValueT::LEFT:
            rotateLeft();
            break;
        case Direction::RelativeValueT::RIGHT:
            rotateRight();
            break;
        case Direction::RelativeValueT::OPPOSED:
            rotate180();
            break;
        }

        direction.MoveInRelativeDirection(relVal);
    }

    template<class T>
    void RelativeRangePattern<T>::Rotate(const Direction &requestedDirection)
    {
        Rotate(direction.GetRelativeDirection(requestedDirection));
    }

    template<class T>
    using AbsoluteRangePattern = RangePatternBase<GridPosition, T>;
}