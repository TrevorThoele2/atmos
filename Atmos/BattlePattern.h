#pragma once

#include "RangePattern.h"

namespace Atmos
{
    namespace Battle
    {
        template<class T>
        class Pattern
        {
        public:
            typedef RelativeRangePattern<T> BasePattern;

            typedef typename BasePattern::iterator iterator;
            typedef typename BasePattern::const_iterator const_iterator;
            typedef typename BasePattern::KeyT KeyT;
            typedef typename BasePattern::ValueT ValueT;
        private:
            GridPosition position;
            RelativeRangePattern<T> pattern;

            virtual void SetPiecePosition(const GridPosition &center, const KeyT &relPos, ValueT &value) = 0;
        public:
            void AddPiece(const KeyT &position, const ValueT &value);

            void Setup(const GridPosition &position, const BasePattern &copy);
            void SetupDiamond(const GridPosition &position, size_t size, const ValueT &copy);
            void Clear();

            bool IsWithin(const KeyT &pos) const;

            void SetPosition(const GridPosition &position);
            const GridPosition& GetPosition() const;

            iterator begin();
            iterator end();
            const_iterator begin() const;
            const_iterator end() const;
        };

        template<class T>
        void Pattern<T>::AddPiece(const KeyT &position, const ValueT &value)
        {
            pattern.Add(position, value);
        }

        template<class T>
        void Pattern<T>::Setup(const GridPosition &position, const BasePattern &copy)
        {
            pattern = copy;
            SetPosition(position);
        }

        template<class T>
        void Pattern<T>::SetupDiamond(const GridPosition &position, size_t size, const ValueT &copy)
        {
            bool xGoingPositive = true;
            bool yGoingPositive = false;

            RelativeGridPosition::ValueT loop = 0;
            RelativeGridPosition::ValueT xPart = 0;
            RelativeGridPosition::ValueT yPart = 0;
            size_t subCount = 1;
            size_t subCountPos = 0;
            while (true)
            {
                pattern.Add(RelativeGridPosition(xPart, yPart, 0), copy);

                ++subCountPos;
                if (subCountPos >= subCount)
                {
                    ++loop;
                    if (loop > static_cast<RelativeGridPosition::ValueT>(size))
                        break;

                    subCount = loop * 4;
                    subCountPos = 0;
                    xPart = -loop;
                    yPart = 0;

                    xGoingPositive = true;
                    yGoingPositive = false;
                }
                else
                {
                    if (xGoingPositive)
                        ++xPart;
                    else
                        --xPart;

                    if (yGoingPositive)
                        ++yPart;
                    else
                        --yPart;

                    if (xPart == loop || xPart == -loop)
                        xGoingPositive = !xGoingPositive;

                    if (yPart == loop || yPart == -loop)
                        yGoingPositive = !yGoingPositive;
                }
            }

            SetPosition(position);
        }

        template<class T>
        void Pattern<T>::Clear()
        {
            pattern.Clear();
        }

        template<class T>
        bool Pattern<T>::IsWithin(const KeyT &pos) const
        {
            return pattern.IsWithin(pos);
        }

        template<class T>
        void Pattern<T>::SetPosition(const GridPosition &position)
        {
            for (auto &loop : pattern)
                SetPiecePosition(position, loop.first, loop.second);

            this->position = position;
        }

        template<class T>
        const GridPosition& Pattern<T>::GetPosition() const
        {
            return position;
        }

        template<class T>
        typename Pattern<T>::iterator Pattern<T>::begin()
        {
            return pattern.begin();
        }

        template<class T>
        typename Pattern<T>::iterator Pattern<T>::end()
        {
            return pattern.end();
        }

        template<class T>
        typename Pattern<T>::const_iterator Pattern<T>::begin() const
        {
            return pattern.begin();
        }

        template<class T>
        typename Pattern<T>::const_iterator Pattern<T>::end() const
        {
            return pattern.end();
        }
    }
}