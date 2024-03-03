#pragma once

#include "ObjectOffset.h"
#include "Assert.h"
#include "Serialization.h"

namespace Atmos
{
    // Manages the object by itself in lieu of a collection
    template<class T, class Position>
    class ObjectOffsetHandle
    {
    public:
        typedef T ObjectT;
        typedef Position PositionT;
        typedef ObjectOffset<PositionT> OffsetT;
        typedef typename OffsetT::ValueT OffsetValueT;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        ObjectT object;
        const PositionT *ownerPosition;
        OffsetValueT x, y, z;

        void SetObjectPosition();
        void SetObjectPosition(const PositionT &position);
    public:
        ObjectOffsetHandle();
        ObjectOffsetHandle(const PositionT &ownerPosition);
        ObjectOffsetHandle(const ObjectT &object, const OffsetT &offset, const PositionT *ownerPosition = nullptr);
        ObjectOffsetHandle(const ObjectT &object, OffsetValueT x = 0, OffsetValueT y = 0, OffsetValueT z = 0, const PositionT *ownerPosition = nullptr);
        ObjectOffsetHandle(ObjectT &&object, const OffsetT &offset, const PositionT *ownerPosition = nullptr);
        ObjectOffsetHandle(ObjectT &&object, OffsetValueT x = 0, OffsetValueT y = 0, OffsetValueT z = 0, const PositionT *ownerPosition = nullptr);

        ObjectOffsetHandle(const ObjectOffsetHandle &arg) = default;
        ObjectOffsetHandle(ObjectOffsetHandle &&arg);
        ObjectOffsetHandle& operator=(const ObjectOffsetHandle &arg) = default;
        ObjectOffsetHandle& operator=(ObjectOffsetHandle &&arg);

        bool operator==(const ObjectOffsetHandle &arg) const;
        bool operator!=(const ObjectOffsetHandle &arg) const;

        ObjectT& operator*();
        ObjectT* operator->();
        const ObjectT& operator*() const;
        const ObjectT* operator->() const;

        void InformPositionChange(const PositionT &position);
        // Use if the owner position is not set
        void InformOffsetChange(const PositionT &position, const OffsetT &offset);
        // Use if the owner position is not set
        void InformOffsetChange(const PositionT &position, OffsetValueT x, OffsetValueT y, OffsetValueT z);
        // Use if the owner position is not set
        void InformOffsetXChange(const PositionT &position, OffsetValueT set);
        // Use if the owner position is not set
        void InformOffsetYChange(const PositionT &position, OffsetValueT set);
        // Use if the owner position is not set
        void InformOffsetZChange(const PositionT &position, OffsetValueT set);
        // Use if the owner position is set
        void InformOffsetChange(const OffsetT &offset);
        // Use if the owner position is set
        void InformOffsetChange(OffsetValueT x, OffsetValueT y, OffsetValueT z);
        // Use if the owner position is set
        void InformOffsetXChange(OffsetValueT set);
        // Use if the owner position is set
        void InformOffsetYChange(OffsetValueT set);
        // Use if the owner position is set
        void InformOffsetZChange(OffsetValueT set);
        // Use if the owner position is set
        void InformOwnerPositionChange();

        // The position set here will not be serialized. It must be set again after serialization
        void SetOwnerPosition(const PositionT *set);

        ObjectT& Get();
        const ObjectT& Get() const;

        const PositionT* GetOwnerPosition() const;
        OffsetValueT GetXOffset() const;
        OffsetValueT GetYOffset() const;
        OffsetValueT GetZOffset() const;
    };

    template<class T, class Position>
    void ObjectOffsetHandle<T, Position>::Serialize(::Inscription::Scribe &scribe)
    {
        scribe(object);
        scribe(x);
        scribe(y);
        scribe(z);
    }

    template<class T, class Position>
    void ObjectOffsetHandle<T, Position>::SetObjectPosition()
    {
        ATMOS_ASSERT_MESSAGE(ownerPosition, "Owner position was not set, and must be at this point.");
        object.SetPosition(PositionT(ownerPosition->GetX() + x, ownerPosition->GetY() + y, ownerPosition->GetZ() + z));
    }

    template<class T, class Position>
    void ObjectOffsetHandle<T, Position>::SetObjectPosition(const PositionT &position)
    {
        object.SetPosition(PositionT(position.GetX() + x, position.GetY() + y, position.GetZ() + z));
    }

    template<class T, class Position>
    ObjectOffsetHandle<T, Position>::ObjectOffsetHandle() : ownerPosition(nullptr), x(0), y(0), z(0)
    {}

    template<class T, class Position>
    ObjectOffsetHandle<T, Position>::ObjectOffsetHandle(const PositionT &ownerPosition) : ownerPosition(&ownerPosition), x(0), y(0), z(0)
    {}

    template<class T, class Position>
    ObjectOffsetHandle<T, Position>::ObjectOffsetHandle(const ObjectT &object, const OffsetT &offset, const PositionT *ownerPosition) : object(object), x(offset.x), y(offset.y), z(offset.z), ownerPosition(ownerPosition)
    {}

    template<class T, class Position>
    ObjectOffsetHandle<T, Position>::ObjectOffsetHandle(const ObjectT &object, OffsetValueT x, OffsetValueT y, OffsetValueT z, const PositionT *ownerPosition) : object(object), x(x), y(y), z(z), ownerPosition(ownerPosition)
    {}

    template<class T, class Position>
    ObjectOffsetHandle<T, Position>::ObjectOffsetHandle(ObjectT &&object, const OffsetT &offset, const PositionT *ownerPosition) : object(std::move(object)), x(offset.x), y(offset.y), z(offset.z), ownerPosition(ownerPosition)
    {}

    template<class T, class Position>
    ObjectOffsetHandle<T, Position>::ObjectOffsetHandle(ObjectT &&object, OffsetValueT x, OffsetValueT y, OffsetValueT z, const PositionT *ownerPosition) : object(std::move(object)), x(x), y(y), z(z), ownerPosition(ownerPosition)
    {}

    template<class T, class Position>
    ObjectOffsetHandle<T, Position>::ObjectOffsetHandle(ObjectOffsetHandle &&arg) : object(std::move(arg.object)), ownerPosition(std::move(arg.ownerPosition)), x(std::move(arg.x)), y(std::move(arg.y)), z(std::move(arg.z))
    {}

    template<class T, class Position>
    ObjectOffsetHandle<T, Position>& ObjectOffsetHandle<T, Position>::operator=(ObjectOffsetHandle &&arg)
    {
        object = std::move(arg.object);
        ownerPosition = std::move(arg.ownerPosition);
        x = std::move(arg.x);
        y = std::move(arg.y);
        z = std::move(arg.z);
        return *this;
    }

    template<class T, class Position>
    bool ObjectOffsetHandle<T, Position>::operator==(const ObjectOffsetHandle &arg) const
    {
        return object == arg.object && ownerPosition == arg.ownerPosition && x == arg.x && y == arg.y && z == arg.z;
    }

    template<class T, class Position>
    bool ObjectOffsetHandle<T, Position>::operator!=(const ObjectOffsetHandle &arg) const
    {
        return !(*this == arg);
    }

    template<class T, class Position>
    typename ObjectOffsetHandle<T, Position>::ObjectT& ObjectOffsetHandle<T, Position>::operator*()
    {
        return object;
    }

    template<class T, class Position>
    typename ObjectOffsetHandle<T, Position>::ObjectT* ObjectOffsetHandle<T, Position>::operator->()
    {
        return &object;
    }

    template<class T, class Position>
    typename const ObjectOffsetHandle<T, Position>::ObjectT& ObjectOffsetHandle<T, Position>::operator*() const
    {
        return object;
    }

    template<class T, class Position>
    typename const ObjectOffsetHandle<T, Position>::ObjectT* ObjectOffsetHandle<T, Position>::operator->() const
    {
        return &object;
    }

    template<class T, class Position>
    void ObjectOffsetHandle<T, Position>::InformPositionChange(const PositionT &position)
    {
        object.SetPosition(PositionT(position.GetX() + x, position.GetY() + y, position.GetZ() + z));
    }

    template<class T, class Position>
    void ObjectOffsetHandle<T, Position>::InformOffsetChange(const PositionT &position, const OffsetT &offset)
    {
        x = offset.x;
        y = offset.y;
        z = offset.z;
        SetObjectPosition(position);
    }

    template<class T, class Position>
    void ObjectOffsetHandle<T, Position>::InformOffsetChange(const PositionT &position, OffsetValueT x, OffsetValueT y, OffsetValueT z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        SetObjectPosition(position);
    }

    template<class T, class Position>
    void ObjectOffsetHandle<T, Position>::InformOffsetXChange(const PositionT &position, OffsetValueT set)
    {
        x = set;
        SetObjectPosition(position);
    }

    template<class T, class Position>
    void ObjectOffsetHandle<T, Position>::InformOffsetYChange(const PositionT &position, OffsetValueT set)
    {
        y = set;
        SetObjectPosition(position);
    }

    template<class T, class Position>
    void ObjectOffsetHandle<T, Position>::InformOffsetZChange(const PositionT &position, OffsetValueT set)
    {
        z = set;
        SetObjectPosition(position);
    }

    template<class T, class Position>
    void ObjectOffsetHandle<T, Position>::InformOffsetChange(const OffsetT &offset)
    {
        x = offset.x;
        y = offset.y;
        z = offset.z;
        SetObjectPosition();
    }

    template<class T, class Position>
    void ObjectOffsetHandle<T, Position>::InformOffsetChange(OffsetValueT x, OffsetValueT y, OffsetValueT z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        SetObjectPosition();
    }

    template<class T, class Position>
    void ObjectOffsetHandle<T, Position>::InformOffsetXChange(OffsetValueT set)
    {
        x = set;
        SetObjectPosition();
    }

    template<class T, class Position>
    void ObjectOffsetHandle<T, Position>::InformOffsetYChange(OffsetValueT set)
    {
        y = set;
        SetObjectPosition();
    }

    template<class T, class Position>
    void ObjectOffsetHandle<T, Position>::InformOffsetZChange(OffsetValueT set)
    {
        z = set;
        SetObjectPosition();
    }

    template<class T, class Position>
    void ObjectOffsetHandle<T, Position>::InformOwnerPositionChange()
    {
        SetObjectPosition();
    }

    template<class T, class Position>
    void ObjectOffsetHandle<T, Position>::SetOwnerPosition(const PositionT *set)
    {
        ownerPosition = set;
        InformOwnerPositionChange();
    }

    template<class T, class Position>
    typename ObjectOffsetHandle<T, Position>::ObjectT& ObjectOffsetHandle<T, Position>::Get()
    {
        return object;
    }

    template<class T, class Position>
    typename const ObjectOffsetHandle<T, Position>::ObjectT& ObjectOffsetHandle<T, Position>::Get() const
    {
        return object;
    }

    template<class T, class Position>
    typename const ObjectOffsetHandle<T, Position>::PositionT* ObjectOffsetHandle<T, Position>::GetOwnerPosition() const
    {
        return ownerPosition;
    }

    template<class T, class Position>
    typename ObjectOffsetHandle<T, Position>::OffsetValueT ObjectOffsetHandle<T, Position>::GetXOffset() const
    {
        return x;
    }

    template<class T, class Position>
    typename ObjectOffsetHandle<T, Position>::OffsetValueT ObjectOffsetHandle<T, Position>::GetYOffset() const
    {
        return y;
    }

    template<class T, class Position>
    typename ObjectOffsetHandle<T, Position>::OffsetValueT ObjectOffsetHandle<T, Position>::GetZOffset() const
    {
        return z;
    }
}