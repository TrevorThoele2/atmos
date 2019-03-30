#pragma once

#include <type_traits>

#include "Object.h"
#include "ObjectReference.h"

#include "Position3D.h"
#include "Size3D.h"
#include "AxisBoundingBox3D.h"
#include "StoredProperty.h"

#include <Chroma/Event.h>

#include "ObjectSerialization.h"

namespace Atmos
{
    class PositionalObject : public Object
    {
    public:
        class Position
        {
        public:
            typedef StoredProperty<Position3D::ValueT> Property;
        public:
            Property x;
            Property y;
            Property z;
        public:
            ::Chroma::Event<Position3D> onValueChanged;
        public:
            Position();

            Position& operator=(const Position3D& arg);

            operator Position3D() const;

            Position3D Get() const;
        private:
            Position3D underlying;
        private:
            Position(const Position& arg);
            Position(Position&& arg);

            Position& operator=(const Position& arg);
            Position& operator=(Position&& arg);

            void Calculate();
        private:
            PositionalObject* owner;
        private:
            void SubscribeToProperties();
            void OnPropertyChanged(Position3D::ValueT value);
        private:
            friend PositionalObject;
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        };

        class Size
        {
        public:
            typedef StoredProperty<Size3D::ValueT> ValueProperty;
            typedef StoredProperty<Radians> AngleProperty;
        public:
            ValueProperty width;
            ValueProperty height;
            ValueProperty depth;

            ValueProperty widthScaler;
            ValueProperty heightScaler;
            ValueProperty depthScaler;

            AngleProperty xRotation;
            AngleProperty yRotation;
            AngleProperty zRotation;
        public:
            ::Chroma::Event<Size3D> onValueChanged;
        public:
            operator Size3D() const;

            Size3D Get() const;
        private:
            Size3D underlying;
        public:
            Size();
        private:
            Size(const Size& arg);
            Size(Size&& arg);

            Size& operator=(const Size& arg);
            Size& operator=(Size&& arg);

            void Calculate();
        private:
            PositionalObject* owner;
        private:
            void SubscribeToProperties();
            template<class T>
            void OnPropertyChanged(T value);
        private:
            friend PositionalObject;
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        };

        class Bounds
        {
        public:
            operator AxisBoundingBox3D() const;

            AxisBoundingBox3D Get() const;
        private:
            AxisBoundingBox3D underlying;
        private:
            friend PositionalObject;
        };
    public:
        Event<TypedObjectReference<PositionalObject>, Position3D> onPositionChanged;
        Event<TypedObjectReference<PositionalObject>, Size3D> onSizeChanged;
        Event<TypedObjectReference<PositionalObject>, AxisBoundingBox3D> onBoundsChanged;
    public:
        Position position;
        Size size;
        Bounds bounds;
    public:
        PositionalObject();
        PositionalObject(const PositionalObject& arg);
        PositionalObject(PositionalObject&& arg);
        PositionalObject(const ::Inscription::Table<PositionalObject>& table);

        virtual ~PositionalObject() = 0;

        PositionalObject& operator=(const PositionalObject& arg);
        PositionalObject& operator=(PositionalObject&& arg);

        ObjectTypeDescription TypeDescription() const override;
    private:
        void CalculateBounds();
    private:
        TypedObjectReference<PositionalObject> ToReference();
    private:
        void LinkAll();
        void PositionHasChanged(Position3D previous);
        void SizeHasChanged(Size3D previous);
        void BoundsHasChanged(AxisBoundingBox3D previous);
    private:
        friend Position;
    private:
        INSCRIPTION_ACCESS;
    };

    template<class T>
    void PositionalObject::Size::OnPropertyChanged(T value)
    {
        Calculate();
        onValueChanged(underlying);
    }

    template<>
    struct ObjectTraits<PositionalObject> : ObjectTraitsBase<PositionalObject>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::PositionalObject)
    {
    public:
        static void AddMembers(TableT& table);

        INSCRIPTION_INSCRIPTER_DECLARE_SERIALIZE_FUNCTION;
    };
}