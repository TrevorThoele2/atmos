#pragma once

/////////////////////
#include "Object.h"
#include "ObjectReference.h"
#include "ObjectException.h"

#include "PositionalObject.h"

#include "StoredProperty.h"

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

    class PositionalOffsetAdapterSourceNotSetException : public ObjectException
    {
    public:
        PositionalOffsetAdapterSourceNotSetException();
    };

    template<class T>
    class PositionalOffsetAdapter
    {
    public:
        typedef TypedObjectReference<T> SourceReference;
    public:
        class Position
        {
        public:
            typedef StoredProperty<Position3D::ValueT> Property;
        public:
            Property xOffset;
            Property yOffset;
            Property zOffset;
        public:
            operator Position3D() const;

            Position3D Get() const;
        private:
            Position3D underlying;
        private:
            Position();
            Position(const Position& arg);
            Position(Position&& arg);

            Position& operator=(const Position& arg);
            Position& operator=(Position&& arg);

            void Calculate();
        private:
            void SubscribeToProperties();
            void OnPropertyChanged(Position3D::ValueT value);
        private:
            PositionalOffsetAdapter* owner;
        private:
            SourceReference source;
            EventScopedConnection sourceConnection;

            void SetSource();
            void SetSource(SourceReference set);
            void OnSourcePositionChanged(TypedObjectReference<PositionalObject> object, Position3D previous);
        private:
            friend PositionalOffsetAdapter;
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        };
    public:
        ::Chroma::Event<Position3D> onPositionChanged;
    public:
        Position position;
    public:
        typedef StoredProperty<SourceReference> SourceProperty;
        SourceProperty source;
    public:
        PositionalOffsetAdapter();
        PositionalOffsetAdapter(const PositionalOffsetAdapter& arg);
        PositionalOffsetAdapter(PositionalOffsetAdapter&& arg);

        PositionalOffsetAdapter& operator=(const PositionalOffsetAdapter& arg);
        PositionalOffsetAdapter& operator=(PositionalOffsetAdapter&& arg);
    private:
        void SubscribeToProperties();
    private:
        void LinkPosition();
        void CalculatePosition();
        void PositionHasChanged(Position3D dispatch);
    private:
        void OnSourceChanged(SourceReference newValue);
    private:
        friend Position;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        static_assert(std::is_base_of<PositionalObject, T>::value, "This must be derived from PositionalObject");
    };

    template<class T>
    PositionalOffsetAdapter<T>::Position::operator Position3D() const
    {
        return Get();
    }

    template<class T>
    Position3D PositionalOffsetAdapter<T>::Position::Get() const
    {
        if (!source)
            throw PositionalOffsetAdapterSourceNotSetException();

        return underlying;
    }

    template<class T>
    PositionalOffsetAdapter<T>::Position::Position() :
        xOffset(0), yOffset(0), zOffset(0), owner(nullptr)
    {
        SubscribeToProperties();
    }

    template<class T>
    PositionalOffsetAdapter<T>::Position::Position(const Position& arg) :
        source(arg.source), xOffset(arg.xOffset), yOffset(arg.yOffset), zOffset(arg.zOffset), owner(nullptr)
    {
        SubscribeToProperties();
    }

    template<class T>
    PositionalOffsetAdapter<T>::Position::Position(Position&& arg) :
        source(std::move(arg.source)), xOffset(std::move(arg.xOffset)), yOffset(std::move(arg.yOffset)), zOffset(std::move(arg.zOffset)), owner(nullptr)
    {
        SubscribeToProperties();
    }

    template<class T>
    typename PositionalOffsetAdapter<T>::Position& PositionalOffsetAdapter<T>::Position::operator=(const Position& arg)
    {
        source = arg.source;
        xOffset = arg.xOffset;
        yOffset = arg.yOffset;
        zOffset = arg.zOffset;
        Calculate();
        return *this;
    }

    template<class T>
    typename PositionalOffsetAdapter<T>::Position& PositionalOffsetAdapter<T>::Position::operator=(Position&& arg)
    {
        source = std::move(arg.source);
        xOffset = std::move(arg.xOffset);
        yOffset = std::move(arg.yOffset);
        zOffset = std::move(arg.zOffset);
        Calculate();
        return *this;
    }

    template<class T>
    void PositionalOffsetAdapter<T>::Position::Calculate()
    {
        if (!source)
            return;

        underlying.SetX(source->position.x + xOffset);
        underlying.SetY(source->position.y + yOffset);
        underlying.SetZ(source->position.z + zOffset);
        owner->PositionHasChanged(underlying);
    }

    template<class T>
    void PositionalOffsetAdapter<T>::Position::SubscribeToProperties()
    {
        xOffset.onValueChanged.Subscribe(&Position::OnPropertyChanged, *this);
        yOffset.onValueChanged.Subscribe(&Position::OnPropertyChanged, *this);
        zOffset.onValueChanged.Subscribe(&Position::OnPropertyChanged, *this);
    }

    template<class T>
    void PositionalOffsetAdapter<T>::Position::OnPropertyChanged(Position3D::ValueT value)
    {
        Calculate();
    }

    template<class T>
    void PositionalOffsetAdapter<T>::Position::SetSource()
    {
        source = SourceReference();
        sourceConnection.Sever();
    }

    template<class T>
    void PositionalOffsetAdapter<T>::Position::SetSource(SourceReference set)
    {
        source = set;
        sourceConnection.Sever();
        if (set.IsOccupied())
            sourceConnection.Set(source->onPositionChanged.Subscribe(&Position::OnSourcePositionChanged, *this));
    }

    template<class T>
    void PositionalOffsetAdapter<T>::Position::OnSourcePositionChanged(TypedObjectReference<PositionalObject> object, Position3D previous)
    {
        Calculate();
    }

    template<class T>
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(PositionalOffsetAdapter<T>::Position)
    {
        scribe(xOffset);
        scribe(yOffset);
        scribe(zOffset);
        scribe(source);
    }

    template<class T>
    PositionalOffsetAdapter<T>::PositionalOffsetAdapter()
    {
        LinkPosition();
        CalculatePosition();
        SubscribeToProperties();
    }

    template<class T>
    PositionalOffsetAdapter<T>::PositionalOffsetAdapter(const PositionalOffsetAdapter& arg) : position(arg.position), source(arg.source)
    {
        LinkPosition();
        CalculatePosition();
        SubscribeToProperties();
    }

    template<class T>
    PositionalOffsetAdapter<T>::PositionalOffsetAdapter(PositionalOffsetAdapter&& arg) : position(std::move(arg.position)), source(std::move(arg.source))
    {
        LinkPosition();
        CalculatePosition();
        SubscribeToProperties();
    }

    template<class T>
    PositionalOffsetAdapter<T>& PositionalOffsetAdapter<T>::operator=(const PositionalOffsetAdapter& arg)
    {
        position = arg.position;
        source = arg.source;
        return *this;
    }

    template<class T>
    PositionalOffsetAdapter<T>& PositionalOffsetAdapter<T>::operator=(PositionalOffsetAdapter&& arg)
    {
        position = std::move(arg.position);
        source = std::move(arg.source);
        return *this;
    }

    template<class T>
    void PositionalOffsetAdapter<T>::SubscribeToProperties()
    {
        source.onValueChanged.Subscribe(&PositionalOffsetAdapter::OnSourceChanged, *this);
    }

    template<class T>
    void PositionalOffsetAdapter<T>::LinkPosition()
    {
        position.owner = this;
    }

    template<class T>
    void PositionalOffsetAdapter<T>::CalculatePosition()
    {
        position.Calculate();
    }

    template<class T>
    void PositionalOffsetAdapter<T>::PositionHasChanged(Position3D dispatch)
    {
        onPositionChanged(dispatch);
    }

    template<class T>
    void PositionalOffsetAdapter<T>::OnSourceChanged(SourceReference newValue)
    {
        position.SetSource(newValue);
    }

    template<class T>
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(PositionalOffsetAdapter<T>)
    {
        scribe(source);
    }
}