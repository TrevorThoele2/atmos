#include "AxisAlignedObject.h"

namespace Atmos
{
    AxisAlignedObject::Position& AxisAlignedObject::Position::operator=(const Position3D& arg)
    {
        _x = arg.x;
        _y = arg.y;
        _z = arg.z;
        Calculate();
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
    }

    void AxisAlignedObject::Position::Calculate()
    {
        real.x = _x;
        real.y = _y;
        real.z = _z;

        if(owner)
            owner->PositionHasChanged(real);
    }

    INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DEFINE(AxisAlignedObject::Position)
    {
        scribe(_x);
        scribe(_y);
        scribe(_z);
        if (scribe.IsInput())
            Calculate();
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
        _baseWidth(std::move(arg._baseWidth)), _baseHeight(std::move(arg._baseHeight)), _baseDepth(std::move(arg._baseDepth)),
        _widthScaler(std::move(arg._widthScaler)), _heightScaler(std::move(arg._heightScaler)), _depthScaler(std::move(arg._depthScaler))
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

        if (owner)
            owner->SizeHasChanged(real);
    }

    void AxisAlignedObject::Size::SetScaler(ScalerValue to, ScalerValue& scalerValue)
    {
        scalerValue = to;
        Calculate();
    }

    void AxisAlignedObject::Size::SetScalerFromValue(Value newValue, Value baseSizeValue, ScalerValue& scalerValue)
    {
        scalerValue = newValue / baseSizeValue;
        Calculate();
    }

    void AxisAlignedObject::Size::BaseSizeChanged(Value width, Value height, Value depth)
    {
        real.baseWidth = width;
        real.baseHeight = height;
        real.baseDepth = depth;

        Calculate();
    }

    INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DEFINE(AxisAlignedObject::Size)
    {
        scribe(_baseWidth);
        scribe(_baseHeight);
        scribe(_baseDepth);
        scribe(_widthScaler);
        scribe(_heightScaler);
        scribe(_depthScaler);
        if (scribe.IsInput())
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

    AxisAlignedObject::AxisAlignedObject(ObjectManager& manager) : Object(manager)
    {
        LinkAll();
        CalculateBounds();
    }

    AxisAlignedObject::AxisAlignedObject(const AxisAlignedObject& arg) : Object(arg), position(arg.position)
    {
        LinkAll();
        CalculateBounds();
    }

    AxisAlignedObject::AxisAlignedObject(AxisAlignedObject&& arg) : Object(std::move(arg)), position(std::move(arg.position))
    {
        LinkAll();
        CalculateBounds();
    }

    INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(AxisAlignedObject) :
        INSCRIPTION_TABLE_GET_BASE(Object), INSCRIPTION_TABLE_GET_MEM(position)
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
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::AxisAlignedObject)
    {
        INSCRIPTION_TABLE_ADD(position);
        INSCRIPTION_TABLE_ADD(size);
    }

    INSCRIPTION_BINARY_INSCRIPTER_DEFINE_SERIALIZE_FUNCTION(::Atmos::AxisAlignedObject)
    {
        INSCRIPTION_INSCRIPTER_CALL_BASE_SERIALIZE_FUNCTION;
        obj.position.Calculate();
        obj.size.Calculate();
        obj.CalculateBounds();
    }
}