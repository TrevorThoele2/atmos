#include "AxisAlignedObject.h"

namespace Atmos
{
    AxisAlignedObject::Position& AxisAlignedObject::Position::operator=(const Position3D& arg)
    {
        _x = arg.x;
        _y = arg.y;
        _z = arg.z;
        Calculate();
        SignalOwnerThisChanged();
        return *this;
    }

    AxisAlignedObject::Position::operator Position3D() const
    {
        return Get();
    }

    Position3D AxisAlignedObject::Position::Get() const
    {
        return real;
    }

    AxisAlignedObject::Position::Position()
    {
        Calculate();
    }

    AxisAlignedObject::Position::Position(const Position& arg) :
        _x(arg._x), _y(arg._y), _z(arg._z)
    {
        Calculate();
    }

    AxisAlignedObject::Position::Position(Position&& arg) :
        _x(std::move(arg._x)), _y(std::move(arg._y)), _z(std::move(arg._z))
    {
        Calculate();
    }

    AxisAlignedObject::Position& AxisAlignedObject::Position::operator=(const Position& arg)
    {
        _x = arg._x;
        _y = arg._y;
        _z = arg._z;
        Calculate();
        return *this;
    }

    AxisAlignedObject::Position& AxisAlignedObject::Position::operator=(Position&& arg)
    {
        _x = std::move(arg._x);
        _y = std::move(arg._y);
        _z = std::move(arg._z);
        Calculate();
        return *this;
    }

    void AxisAlignedObject::Position::SetValue(Value to, Value& value)
    {
        value = to;
        Calculate();
        SignalOwnerThisChanged();
    }

    void AxisAlignedObject::Position::Calculate()
    {
        real.x = _x;
        real.y = _y;
        real.z = _z;
    }

    void AxisAlignedObject::Position::SignalOwnerThisChanged()
    {
        if (owner)
            owner->PositionHasChanged(real);
    }

    AxisAlignedObject::Size& AxisAlignedObject::Size::operator=(const Size3D& arg)
    {
        _baseWidth = arg.baseWidth;
        _baseHeight = arg.baseHeight;
        _baseDepth = arg.baseDepth;
        _widthScaler = arg.widthScaler;
        _heightScaler = arg.heightScaler;
        _depthScaler = arg.depthScaler;
        Calculate();
        SignalOwnerThisChanged();
        return *this;
    }

    AxisAlignedObject::Size::operator Size3D() const
    {
        return Get();
    }

    Size3D AxisAlignedObject::Size::Get() const
    {
        return real;
    }

    AxisAlignedObject::Size::Size() :
        _widthScaler(0.0f), _heightScaler(0.0f), _depthScaler(0.0f)
    {
        Calculate();
    }

    AxisAlignedObject::Size::Size(const Size& arg) :
        _baseWidth(arg._baseWidth), _baseHeight(arg._baseHeight), _baseDepth(arg._baseDepth),
        _widthScaler(arg._widthScaler), _heightScaler(arg._heightScaler), _depthScaler(arg._depthScaler)
    {
        Calculate();
    }

    AxisAlignedObject::Size::Size(Size&& arg) :
        _baseWidth(std::move(arg._baseWidth)), _baseHeight(std::move(arg._baseHeight)),
        _baseDepth(std::move(arg._baseDepth)),
        _widthScaler(std::move(arg._widthScaler)), _heightScaler(std::move(arg._heightScaler)),
        _depthScaler(std::move(arg._depthScaler))
    {
        Calculate();
    }

    AxisAlignedObject::Size& AxisAlignedObject::Size::operator=(const Size& arg)
    {
        _baseWidth = arg._baseWidth;
        _baseHeight = arg._baseHeight;
        _baseDepth = arg._baseDepth;
        _widthScaler = arg._widthScaler;
        _heightScaler = arg._heightScaler;
        _depthScaler = arg._depthScaler;
        Calculate();
        return *this;
    }

    AxisAlignedObject::Size& AxisAlignedObject::Size::operator=(Size&& arg)
    {
        _baseWidth = std::move(arg._baseWidth);
        _baseHeight = std::move(arg._baseHeight);
        _baseDepth = std::move(arg._baseDepth);
        _widthScaler = std::move(arg._widthScaler);
        _heightScaler = std::move(arg._heightScaler);
        _depthScaler = std::move(arg._depthScaler);
        Calculate();
        return *this;
    }

    void AxisAlignedObject::Size::Calculate()
    {
        real.baseWidth = _baseWidth;
        real.baseHeight = _baseHeight;
        real.baseDepth = _baseDepth;
        real.widthScaler = _widthScaler;
        real.heightScaler = _heightScaler;
        real.depthScaler = _depthScaler;
    }

    void AxisAlignedObject::Size::SignalOwnerThisChanged()
    {
        if (owner)
            owner->SizeHasChanged(real);
    }

    void AxisAlignedObject::Size::SetScaler(ScalerValue to, ScalerValue& scalerValue)
    {
        scalerValue = to;
        Calculate();
        SignalOwnerThisChanged();
    }

    void AxisAlignedObject::Size::SetScalerFromValue(Value newValue, Value baseSizeValue, ScalerValue& scalerValue)
    {
        scalerValue = newValue / baseSizeValue;
        Calculate();
        SignalOwnerThisChanged();
    }

    void AxisAlignedObject::Size::BaseSizeChanged(Value width, Value height, Value depth)
    {
        real.baseWidth = width;
        real.baseHeight = height;
        real.baseDepth = depth;
        Calculate();
    }

    AxisAlignedObject::Bounds::operator AxisAlignedBox3D() const
    {
        return underlying;
    }

    AxisAlignedBox3D AxisAlignedObject::Bounds::Get() const
    {
        return underlying;
    }

    AxisAlignedObject::AxisAlignedObject(ObjectManager& manager) :
        Object(manager)
    {
        LinkAll();
        CalculateBounds();
    }

    AxisAlignedObject::AxisAlignedObject(const AxisAlignedObject& arg) :
        Object(arg), position(arg.position)
    {
        LinkAll();
        CalculateBounds();
    }

    AxisAlignedObject::AxisAlignedObject(AxisAlignedObject&& arg) :
        Object(std::move(arg)), position(std::move(arg.position))
    {
        LinkAll();
        CalculateBounds();
    }

    AxisAlignedObject::AxisAlignedObject(const ::Inscription::BinaryTableData<AxisAlignedObject>& data) :
        Object(std::get<0>(data.bases)), position(data.position), size(data.size)
    {
        LinkAll();
        CalculateBounds();
    }

    AxisAlignedObject::~AxisAlignedObject()
    {}

    AxisAlignedObject& AxisAlignedObject::operator=(const AxisAlignedObject& arg)
    {
        position = arg.position;
        return *this;
    }

    AxisAlignedObject& AxisAlignedObject::operator=(AxisAlignedObject&& arg)
    {
        position = std::move(arg.position);
        return *this;
    }

    ObjectTypeDescription AxisAlignedObject::TypeDescription() const
    {
        return ObjectTraits<AxisAlignedObject>::TypeDescription();
    }

    void AxisAlignedObject::InformBaseSizeChanged(Size::Value width, Size::Value height, Size::Value depth)
    {
        size.BaseSizeChanged(width, height, depth);
    }

    void AxisAlignedObject::CalculateBounds()
    {
        bounds.Get().center = position;
        bounds.Get().size = size;
    }

    void AxisAlignedObject::LinkAll()
    {
        position.owner = this;
        size.owner = this;
    }

    void AxisAlignedObject::PositionHasChanged(Position3D previous)
    {
        AxisAlignedBox3D previousBounds(bounds);
        CalculateBounds();

        onPositionChanged(previous);
        BoundsHasChanged(previousBounds);
    }

    void AxisAlignedObject::SizeHasChanged(Size3D previous)
    {
        AxisAlignedBox3D previousBounds(bounds);
        CalculateBounds();

        onSizeChanged(previous);
        BoundsHasChanged(previousBounds);
    }

    void AxisAlignedObject::BoundsHasChanged(AxisAlignedBox3D previous)
    {
        onBoundsChanged(previous);
    }

    const ObjectTypeName ObjectTraits<AxisAlignedObject>::typeName = "PositionalObject";
}

namespace Inscription
{
    void Scribe<::Atmos::AxisAlignedObject::Position, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        archive(object._x);
        archive(object._y);
        archive(object._z);
        if (archive.IsInput())
            object.Calculate();
    }

    void Scribe<::Atmos::AxisAlignedObject::Position, BinaryArchive>::ConstructImplementation(
        ObjectT* storage, ArchiveT& archive)
    {
        DoBasicConstruction(storage, archive);
    }

    void Scribe<::Atmos::AxisAlignedObject::Size, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        archive(object._baseWidth);
        archive(object._baseHeight);
        archive(object._baseDepth);
        archive(object._widthScaler);
        archive(object._heightScaler);
        archive(object._depthScaler);
        if (archive.IsInput())
            object.Calculate();
    }

    void Scribe<::Atmos::AxisAlignedObject::Size, BinaryArchive>::ConstructImplementation(
        ObjectT* storage, ArchiveT& archive)
    {
        DoBasicConstruction(storage, archive);
    }

    Scribe<::Atmos::AxisAlignedObject, BinaryArchive>::Table::Table()
    {
        MergeDataEntries({
            DataEntry::Auto(&ObjectT::position, &DataT::position),
            DataEntry::Auto(&ObjectT::size, &DataT::size) });
    }

    void Scribe<::Atmos::AxisAlignedObject, BinaryArchive>::Table::ObjectScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsInput())
        {
            object.position.Calculate();
            object.size.Calculate();
            object.CalculateBounds();
        }
    }
}