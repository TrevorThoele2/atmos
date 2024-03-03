#pragma once

#include <type_traits>

#include "Object.h"
#include "ObjectReference.h"

#include "Position3D.h"
#include "Size3D.h"
#include "Scalers3D.h"
#include "AxisAlignedBox3D.h"
#include "StoredProperty.h"

#include "Event.h"

#include "ObjectScribe.h"

namespace Atmos
{
    class AxisAlignedObject : public Object
    {
    public:
        class Position
        {
        public:
            typedef Position3D::Value Value;
            typedef Property<Value> Property;

            Property x = Property(
                [this]() { return _x; },
                [this](Value newValue) { SetValue(newValue, _x); });
            Property y = Property(
                [this]() { return _y; },
                [this](Value newValue) { SetValue(newValue, _y); });
            Property z = Property(
                [this]() { return _z; },
                [this](Value newValue) { SetValue(newValue, _z); });
        public:
            Position& operator=(const Position3D& arg);

            operator Position3D() const;
            Position3D Get() const;
        private:
            Value _x;
            Value _y;
            Value _z;

            Position3D real;
        private:
            Position();
            Position(const Position& arg);
            Position(Position&& arg);

            Position& operator=(const Position& arg);
            Position& operator=(Position&& arg);
        private:
            void SetValue(Value to, Value& value);

            void Calculate();
        private:
            AxisAlignedObject* owner;
        private:
            friend AxisAlignedObject;
        private:
            INSCRIPTION_TABLE_ACCESS;
        };

        class Size
        {
        public:
            typedef Size3D::Value Value;
            typedef Property<Value> ValueProperty;

            ValueProperty width = ValueProperty(
                [this]() { return real.scaledWidth; },
                [this](Value newValue) { SetScalerFromValue(newValue, _widthScaler, _widthScaler); });
            ValueProperty height = ValueProperty(
                [this]() { return real.scaledHeight; },
                [this](Value newValue) { SetScalerFromValue(newValue, _heightScaler, _heightScaler); });
            ValueProperty depth = ValueProperty(
                [this]() { return real.scaledDepth; },
                [this](Value newValue) { SetScalerFromValue(newValue, _depthScaler, _depthScaler); });

            typedef Scalers3D::Value ScalerValue;
            typedef Property<ScalerValue> ScalerValueProperty;

            ScalerValueProperty widthScaler = ScalerValueProperty(
                [this]() { return _widthScaler; },
                [this](ScalerValue newValue) { SetScaler(newValue, _widthScaler); });
            ScalerValueProperty heightScaler = ScalerValueProperty(
                [this]() { return _heightScaler; },
                [this](ScalerValue newValue) { SetScaler(newValue, _heightScaler); });
            ScalerValueProperty depthScaler = ScalerValueProperty(
                [this]() { return _depthScaler; },
                [this](ScalerValue newValue) { SetScaler(newValue, _depthScaler); });
        public:
            Size& operator=(const Size3D& arg);

            operator Size3D() const;
            Size3D Get() const;
        private:
            Value _baseWidth;
            Value _baseHeight;
            Value _baseDepth;

            ScalerValue _widthScaler;
            ScalerValue _heightScaler;
            ScalerValue _depthScaler;

            Size3D real;
        private:
            Size();
            Size(const Size& arg);
            Size(Size&& arg);

            Size& operator=(const Size& arg);
            Size& operator=(Size&& arg);
        private:
            void SetScaler(ScalerValue to, ScalerValue& scalerValue);
            void SetScalerFromValue(Value newValue, Value baseSizeValue, ScalerValue& scalerValue);
            void BaseSizeChanged(Value width, Value height, Value depth);

            void Calculate();
        private:
            AxisAlignedObject* owner;
        private:
            friend AxisAlignedObject;
        private:
            INSCRIPTION_TABLE_ACCESS;
        };

        class Bounds
        {
        public:
            operator AxisAlignedBox3D() const;

            AxisAlignedBox3D Get() const;
        private:
            AxisAlignedBox3D underlying;
        private:
            friend AxisAlignedObject;
        };
    public:
        Event<Position3D> onPositionChanged;
        Event<Size3D> onSizeChanged;
        Event<AxisAlignedBox3D> onBoundsChanged;
    public:
        Position position;
        Size size;
        Bounds bounds;
    public:
        AxisAlignedObject(ObjectManager& manager);
        AxisAlignedObject(const AxisAlignedObject& arg);
        AxisAlignedObject(AxisAlignedObject&& arg);
        AxisAlignedObject(const ::Inscription::BinaryTableData<AxisAlignedObject>& data);

        virtual ~AxisAlignedObject() = 0;

        AxisAlignedObject& operator=(const AxisAlignedObject& arg);
        AxisAlignedObject& operator=(AxisAlignedObject&& arg);

        ObjectTypeDescription TypeDescription() const override;
    protected:
        void InformBaseSizeChanged(Size::Value width, Size::Value height, Size::Value depth);
    private:
        void CalculateBounds();
    private:
        void LinkAll();
        void PositionHasChanged(Position3D previous);
        void SizeHasChanged(Size3D previous);
        void BoundsHasChanged(AxisAlignedBox3D previous);
    private:
        friend Position;
    private:
        INSCRIPTION_ACCESS;
    };

    template<>
    struct ObjectTraits<AxisAlignedObject> : ObjectTraitsBase<AxisAlignedObject>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::AxisAlignedObject::Position, BinaryArchive> :
        public CompositeScribe<::Atmos::AxisAlignedObject::Position, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };

    template<>
    class Scribe<::Atmos::AxisAlignedObject::Size, BinaryArchive> :
        public CompositeScribe<::Atmos::AxisAlignedObject::Size, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };

    template<>
    struct TableData<::Atmos::AxisAlignedObject, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::AxisAlignedObject, BinaryArchive>
    {
        ObjectT::Position position;
        ObjectT::Size size;
    };

    template<>
    class Scribe<::Atmos::AxisAlignedObject, BinaryArchive> :
        public ObjectScribe<::Atmos::AxisAlignedObject, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {
        public:
            Table();
        protected:
            void ObjectScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        };
    };
}