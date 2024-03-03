#pragma once

namespace Atmos
{
    template<class Position>
    class ObjectOffset
    {
    public:
        typedef Position PositionT;
        typedef typename PositionT::ValueT ValueT;
    public:
        ValueT x, y, z;
        ObjectOffset();
        ObjectOffset(ValueT x, ValueT y, ValueT z);
        bool operator==(const ObjectOffset &arg) const;
        bool operator!=(const ObjectOffset &arg) const;

        void Set(ValueT x, ValueT y, ValueT z);
        void SetX(ValueT set);
        void SetY(ValueT set);
        void SetZ(ValueT set);

        ValueT GetX() const;
        ValueT GetY() const;
        ValueT GetZ() const;
    };

    template<class Position>
    ObjectOffset<Position>::ObjectOffset() : x(0), y(0), z(0)
    {}

    template<class Position>
    ObjectOffset<Position>::ObjectOffset(ValueT x, ValueT y, ValueT z) : x(x), y(y), z(z)
    {}

    template<class Position>
    bool ObjectOffset<Position>::operator==(const ObjectOffset &arg) const
    {
        return x == arg.x && y == arg.y && z == arg.z;
    }

    template<class Position>
    bool ObjectOffset<Position>::operator!=(const ObjectOffset &arg) const
    {
        return !(*this == arg);
    }

    template<class Position>
    void ObjectOffset<Position>::Set(ValueT x, ValueT y, ValueT z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    template<class Position>
    void ObjectOffset<Position>::SetX(ValueT set)
    {
        x = set;
    }

    template<class Position>
    void ObjectOffset<Position>::SetY(ValueT set)
    {
        y = set;
    }

    template<class Position>
    void ObjectOffset<Position>::SetZ(ValueT set)
    {
        z = set;
    }

    template<class Position>
    typename ObjectOffset<Position>::ValueT ObjectOffset<Position>::GetX() const
    {
        return x;
    }

    template<class Position>
    typename ObjectOffset<Position>::ValueT ObjectOffset<Position>::GetY() const
    {
        return y;
    }

    template<class Position>
    typename ObjectOffset<Position>::ValueT ObjectOffset<Position>::GetZ() const
    {
        return z;
    }
}