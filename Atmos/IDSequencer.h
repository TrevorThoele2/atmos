#pragma once

#include <limits>
#include <exception>

#include "IntervalList.h"

#include "Serialization.h"
#include <Inscription\Scribe.h>
#include <Inscription\Set.h>

namespace Atmos
{
    template<class IDType>
    class IDSequencerBase
    {
    public:
        typedef IDType ID;
        typedef size_t SizeT;
    private:
        typedef IntervalList<ID> Occupied;
    public:
        typedef typename Occupied::const_iterator const_iterator;
    protected:
        IDSequencerBase() = default;
        IDSequencerBase(const IDSequencerBase &arg) = default;
        IDSequencerBase& operator=(const IDSequencerBase &arg) = default;
        IDSequencerBase(IDSequencerBase &&arg);
        IDSequencerBase& operator=(IDSequencerBase &&arg);
        bool operator==(const IDSequencerBase &arg) const;
        bool operator!=(const IDSequencerBase &arg) const;
    public:
        virtual ~IDSequencerBase() = 0 {}

        ID GetNextID() const;
        ID ReserveNext();
        void Reserve(ID id);
        void IDRemoved(ID removed);
        // Returns true if the range given has NOT been exhausted
        bool CanMakeAnother() const;
        bool IsIDTaken(ID check) const;

        void Clear();
        void Reset();

        SizeT Size() const;

        const_iterator begin() const;
        const_iterator end() const;

        virtual ID GetLowerLimit() const = 0;
        virtual ID GetUpperLimit() const = 0;
    protected:
        void SetDirection();
    private:
        enum class Direction : bool
        {
            FORWARD,
            BACKWARD
        };
    private:
        Direction direction;

        Occupied occupied;

        template<Direction dir>
        bool IsDirection() const;
        // The first ID given (when everything is empty)
        ID GetBaseID() const;
        ID GetNewID() const;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };

    template<class IDType>
    IDSequencerBase<IDType>::IDSequencerBase(IDSequencerBase &&arg) : direction(arg.direction), occupied(std::move(arg.occupied))
    {}

    template<class IDType>
    IDSequencerBase<IDType>& IDSequencerBase<IDType>::operator=(IDSequencerBase &&arg)
    {
        direction = arg.direction;
        occupied = std::move(arg.occupied);
        return *this;
    }

    template<class IDType>
    bool IDSequencerBase<IDType>::operator==(const IDSequencerBase &arg) const
    {
        return direction == arg.direction && occupied == arg.occupied;
    }

    template<class IDType>
    bool IDSequencerBase<IDType>::operator!=(const IDSequencerBase &arg) const
    {
        return !(*this == arg);
    }

    template<class IDType>
    typename IDSequencerBase<IDType>::ID IDSequencerBase<IDType>::GetNextID() const
    {
        // If everything is empty
        if (occupied.empty())
            return GetBaseID();

        return GetNewID();
    }

    template<class IDType>
    typename IDSequencerBase<IDType>::ID IDSequencerBase<IDType>::ReserveNext()
    {
        if (!CanMakeAnother())
            throw std::runtime_error("This sequencer is already at capacity and cannot add anymore.");

        ID id = 0;
        // If everything is empty
        if (occupied.IsEmpty())
            id = GetBaseID();
        else
            id = GetNewID();

        Reserve(id);
        return id;
    }

    template<class IDType>
    void IDSequencerBase<IDType>::Reserve(ID id)
    {
        if (!CanMakeAnother())
            return;

        occupied.Include(id);
    }

    template<class IDType>
    void IDSequencerBase<IDType>::IDRemoved(ID removed)
    {
        occupied.Remove(removed);
    }

    template<class IDType>
    bool IDSequencerBase<IDType>::CanMakeAnother() const
    {
        if (GetUpperLimit() == GetLowerLimit())
            return false;

        if (occupied.IsEmpty())
            return true;

        return occupied.rbegin()->GetEnd() != GetUpperLimit();
    }

    template<class IDType>
    bool IDSequencerBase<IDType>::IsIDTaken(ID check) const
    {
        return occupied.IsIncluded(check);
    }

    template<class IDType>
    void IDSequencerBase<IDType>::Clear()
    {
        occupied.Clear();
    }

    template<class IDType>
    void IDSequencerBase<IDType>::Reset()
    {
        Clear();
    }

    template<class IDType>
    typename IDSequencerBase<IDType>::SizeT IDSequencerBase<IDType>::Size() const
    {
        return occupied.Size();
    }

    template<class IDType>
    typename IDSequencerBase<IDType>::const_iterator IDSequencerBase<IDType>::begin() const
    {
        return occupied.begin();
    }

    template<class IDType>
    typename IDSequencerBase<IDType>::const_iterator IDSequencerBase<IDType>::end() const
    {
        return occupied.end();
    }

    template<class IDType>
    void IDSequencerBase<IDType>::SetDirection()
    {
        (GetLowerLimit() < GetUpperLimit()) ? direction = Direction::FORWARD : direction = Direction::BACKWARD;
    }

    template<class IDType>
    template<typename IDSequencerBase<IDType>::Direction dir>
    bool IDSequencerBase<IDType>::IsDirection() const
    {
        return dir == direction;
    }

    template<class IDType>
    typename IDSequencerBase<IDType>::ID IDSequencerBase<IDType>::GetBaseID() const
    {
        return GetLowerLimit();
    }

    template<class IDType>
    typename IDSequencerBase<IDType>::ID IDSequencerBase<IDType>::GetNewID() const
    {
        if (IsDirection<Direction::FORWARD>())
        {
            auto itr = occupied.begin();
            if (itr->GetStart() != GetBaseID())
                return GetBaseID();
            else
                return itr->GetEnd() + 1;
        }
        else
        {
            auto itr = occupied.rbegin();
            if (itr->GetEnd() != GetBaseID())
                return GetBaseID();
            else
                return itr->GetStart() - 1;
        }
    }

    template<class IDType>
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(IDSequencerBase<IDType>)
    {
        scribe(occupied);
    }

    // Keeps track of gaps and fills them in as needed
    template<class IDType>
    class IDSequencer : public IDSequencerBase<IDType>
    {
    private:
        typedef IDSequencerBase<IDType> BaseT;
    public:
        typedef typename BaseT::ID ID;
    public:
        IDSequencer(ID lowerLimit = std::numeric_limits<ID>::min(), ID upperLimit = std::numeric_limits<ID>::max());
        IDSequencer(const IDSequencer &arg) = default;
        IDSequencer& operator=(const IDSequencer &arg) = default;
        IDSequencer(IDSequencer &&arg);
        IDSequencer& operator=(IDSequencer &&arg);
        bool operator==(const IDSequencer &arg) const;
        bool operator!=(const IDSequencer &arg) const;

        void SetLowerLimit(ID set);
        void SetUpperLimit(ID set);
        ID GetLowerLimit() const override;
        ID GetUpperLimit() const override;
    protected:
        using BaseT::SetDirection;
    private:
        ID lowerLimit;
        ID upperLimit;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };

    template<class IDType>
    IDSequencer<IDType>::IDSequencer(ID lowerLimit, ID upperLimit) : lowerLimit(lowerLimit), upperLimit(upperLimit)
    {
        SetDirection();
    }

    template<class IDType>
    IDSequencer<IDType>::IDSequencer(IDSequencer &&arg) : BaseT(std::move(arg)), lowerLimit(arg.lowerLimit), upperLimit(arg.upperLimit)
    {}

    template<class IDType>
    IDSequencer<IDType>& IDSequencer<IDType>::operator=(IDSequencer &&arg)
    {
        BaseT::operator=(std::move(arg));
        lowerLimit = arg.lowerLimit;
        upperLimit = arg.upperLimit;
        return *this;
    }

    template<class IDType>
    bool IDSequencer<IDType>::operator==(const IDSequencer &arg) const
    {
        return BaseT::operator==(arg) && lowerLimit == arg.lowerLimit && upperLimit == arg.upperLimit;
    }

    template<class IDType>
    bool IDSequencer<IDType>::operator!=(const IDSequencer &arg) const
    {
        return !(*this == arg);
    }

    template<class IDType>
    void IDSequencer<IDType>::SetLowerLimit(ID set)
    {
        lowerLimit = set;
        SetDirection();
    }

    template<class IDType>
    void IDSequencer<IDType>::SetUpperLimit(ID set)
    {
        upperLimit = set;
        SetDirection();
    }

    template<class IDType>
    typename IDSequencer<IDType>::ID IDSequencer<IDType>::GetLowerLimit() const
    {
        return lowerLimit;
    }

    template<class IDType>
    typename IDSequencer<IDType>::ID IDSequencer<IDType>::GetUpperLimit() const
    {
        return upperLimit;
    }

    template<class IDType>
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(IDSequencer<IDType>)
    {
        ::Inscription::BaseSerialize<IDSequencerBase<IDType>>(scribe, *this);
        scribe(lowerLimit);
        scribe(upperLimit);

        if (scribe.IsInput())
            SetDirection();
    }

    template<class IDType, IDType lowerLimit = std::numeric_limits<IDType>::min(), IDType upperLimit = std::numeric_limits<IDType>::max()>
    class IDSequencerStatic : public IDSequencerBase<IDType>
    {
    private:
        typedef IDSequencerBase<IDType> BaseT;
    public:
        typedef typename BaseT::ID ID;
    public:
        IDSequencerStatic();
        IDSequencerStatic(const IDSequencerStatic &arg) = default;
        IDSequencerStatic& operator=(const IDSequencerStatic &arg) = default;
        IDSequencerStatic(IDSequencerStatic &&arg);
        IDSequencerStatic& operator=(IDSequencerStatic &&arg);
        bool operator==(const IDSequencerStatic &arg) const;
        bool operator!=(const IDSequencerStatic &arg) const;

        ID GetLowerLimit() const override;
        ID GetUpperLimit() const override;
    protected:
        using BaseT::SetDirection;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };

    template<class IDType, IDType lowerLimit, IDType upperLimit>
    IDSequencerStatic<IDType, lowerLimit, upperLimit>::IDSequencerStatic()
    {
        SetDirection();
    }

    template<class IDType, IDType lowerLimit, IDType upperLimit>
    IDSequencerStatic<IDType, lowerLimit, upperLimit>::IDSequencerStatic(IDSequencerStatic &&arg) : BaseT(std::move(arg))
    {}

    template<class IDType, IDType lowerLimit, IDType upperLimit>
    IDSequencerStatic<IDType, lowerLimit, upperLimit>& IDSequencerStatic<IDType, lowerLimit, upperLimit>::operator=(IDSequencerStatic &&arg)
    {
        BaseT::operator=(std::move(arg));
        return *this;
    }

    template<class IDType, IDType lowerLimit, IDType upperLimit>
    bool IDSequencerStatic<IDType, lowerLimit, upperLimit>::operator==(const IDSequencerStatic &arg) const
    {
        return BaseT::operator==(arg);
    }

    template<class IDType, IDType lowerLimit, IDType upperLimit>
    bool IDSequencerStatic<IDType, lowerLimit, upperLimit>::operator!=(const IDSequencerStatic &arg) const
    {
        return !(*this == arg);
    }

    template<class IDType, IDType lowerLimit, IDType upperLimit>
    typename IDSequencerStatic<IDType, lowerLimit, upperLimit>::ID IDSequencerStatic<IDType, lowerLimit, upperLimit>::GetLowerLimit() const
    {
        return lowerLimit;
    }

    template<class IDType, IDType lowerLimit, IDType upperLimit>
    typename IDSequencerStatic<IDType, lowerLimit, upperLimit>::ID IDSequencerStatic<IDType, lowerLimit, upperLimit>::GetUpperLimit() const
    {
        return upperLimit;
    }

    template<class IDType, IDType lowerLimit, IDType upperLimit>
    void IDSequencerStatic<IDType, lowerLimit, upperLimit>::Serialize(::Inscription::Scribe &scribe)
    {
        ::Inscription::BaseSerialize<IDSequencerBase<IDType>>(scribe, *this);

        if(scribe.IsInput())
            SetDirection();
    }
}