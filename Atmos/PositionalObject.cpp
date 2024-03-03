
#include "PositionalObject.h"

namespace Atmos
{
    PositionalObject::Position::Position()
    {
        SubscribeToProperties();
        Calculate();
    }

    PositionalObject::Position& PositionalObject::Position::operator=(const Position3D& arg)
    {
        x = arg.GetX();
        y = arg.GetY();
        z = arg.GetZ();
        Calculate();
        return *this;
    }

    PositionalObject::Position::operator Position3D() const
    {
        return Get();
    }

    Position3D PositionalObject::Position::Get() const
    {
        return underlying;
    }

    PositionalObject::Position::Position(const Position& arg) : x(arg.x), y(arg.y), z(arg.z)
    {
        SubscribeToProperties();
        Calculate();
    }

    PositionalObject::Position::Position(Position&& arg) : x(std::move(arg.x)), y(std::move(arg.y)), z(std::move(arg.z))
    {
        SubscribeToProperties();
        Calculate();
    }

    PositionalObject::Position& PositionalObject::Position::operator=(const Position& arg)
    {
        x = arg.x;
        y = arg.y;
        z = arg.z;
        Calculate();
        return *this;
    }

    PositionalObject::Position& PositionalObject::Position::operator=(Position&& arg)
    {
        x = std::move(arg.x);
        y = std::move(arg.y);
        z = std::move(arg.z);
        Calculate();
        return *this;
    }

    void PositionalObject::Position::Calculate()
    {
        underlying.SetX(x);
        underlying.SetY(y);
        underlying.SetZ(z);

        if(owner)
            owner->PositionHasChanged(underlying);
    }

    void PositionalObject::Position::SubscribeToProperties()
    {
        x.onValueChanged.Subscribe(&Position::OnPropertyChanged, *this);
        y.onValueChanged.Subscribe(&Position::OnPropertyChanged, *this);
        z.onValueChanged.Subscribe(&Position::OnPropertyChanged, *this);
    }

    void PositionalObject::Position::OnPropertyChanged(Position3D::ValueT value)
    {
        Calculate();
        onValueChanged(underlying);
    }

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(PositionalObject::Position)
    {
        scribe(x);
        scribe(y);
        scribe(z);
        if (scribe.IsInput())
            Calculate();
    }

    PositionalObject::Size::operator Size3D() const
    {
        return Get();
    }

    Size3D PositionalObject::Size::Get() const
    {
        return underlying;
    }

    PositionalObject::Size::Size()
    {
        SubscribeToProperties();
    }

    PositionalObject::Size::Size(const Size& arg) :
        width(arg.width), height(arg.height), depth(arg.depth),
        widthScaler(arg.widthScaler), heightScaler(arg.heightScaler), depthScaler(arg.depthScaler),
        xRotation(arg.xRotation), yRotation(arg.yRotation), zRotation(arg.zRotation)
    {
        SubscribeToProperties();
    }

    PositionalObject::Size::Size(Size&& arg) :
        width(std::move(arg.width)), height(std::move(arg.height)), depth(std::move(arg.depth)),
        widthScaler(std::move(arg.widthScaler)), heightScaler(std::move(arg.heightScaler)), depthScaler(std::move(arg.depthScaler)),
        xRotation(std::move(arg.xRotation)), yRotation(std::move(arg.yRotation)), zRotation(std::move(arg.zRotation))
    {
        SubscribeToProperties();
    }

    PositionalObject::Size& PositionalObject::Size::operator=(const Size& arg)
    {
        width = arg.width;
        height = arg.height;
        depth = arg.depth;
        widthScaler = arg.widthScaler;
        heightScaler = arg.heightScaler;
        depthScaler = arg.depthScaler;
        xRotation = arg.xRotation;
        yRotation = arg.yRotation;
        zRotation = arg.zRotation;
        Calculate();
        return *this;
    }

    PositionalObject::Size& PositionalObject::Size::operator=(Size&& arg)
    {
        width = std::move(arg.width);
        height = std::move(arg.height);
        depth = std::move(arg.depth);
        widthScaler = std::move(arg.widthScaler);
        heightScaler = std::move(arg.heightScaler);
        depthScaler = std::move(arg.depthScaler);
        xRotation = std::move(arg.xRotation);
        yRotation = std::move(arg.yRotation);
        zRotation = std::move(arg.zRotation);
        Calculate();
        return *this;
    }

    void PositionalObject::Size::Calculate()
    {
        underlying.SetWidth(width);
        underlying.SetHeight(height);
        underlying.SetDepth(depth);
        underlying.SetXScaler(widthScaler);
        underlying.SetYScaler(heightScaler);
        underlying.SetZScaler(depthScaler);
        underlying.SetXRotation(Angle(xRotation.Get()));
        underlying.SetYRotation(Angle(yRotation.Get()));
        underlying.SetZRotation(Angle(zRotation.Get()));

        owner->SizeHasChanged(underlying);
    }

    void PositionalObject::Size::SubscribeToProperties()
    {
        typedef ValueProperty::Value ValueT;
        typedef AngleProperty::Value AngleT;

        width.onValueChanged.Subscribe(&Size::OnPropertyChanged<ValueT>, *this);
        height.onValueChanged.Subscribe(&Size::OnPropertyChanged<ValueT>, *this);
        depth.onValueChanged.Subscribe(&Size::OnPropertyChanged<ValueT>, *this);
        widthScaler.onValueChanged.Subscribe(&Size::OnPropertyChanged<ValueT>, *this);
        heightScaler.onValueChanged.Subscribe(&Size::OnPropertyChanged<ValueT>, *this);
        depthScaler.onValueChanged.Subscribe(&Size::OnPropertyChanged<ValueT>, *this);
        xRotation.onValueChanged.Subscribe(&Size::OnPropertyChanged<AngleT>, *this);
        yRotation.onValueChanged.Subscribe(&Size::OnPropertyChanged<AngleT>, *this);
        zRotation.onValueChanged.Subscribe(&Size::OnPropertyChanged<AngleT>, *this);
    }

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(PositionalObject::Size)
    {
        scribe(width);
        scribe(height);
        scribe(depth);
        scribe(widthScaler);
        scribe(heightScaler);
        scribe(depthScaler);
        scribe(xRotation);
        scribe(yRotation);
        scribe(zRotation);
    }

    PositionalObject::Bounds::operator AxisBoundingBox3D() const
    {
        return underlying;
    }

    AxisBoundingBox3D PositionalObject::Bounds::Get() const
    {
        return underlying;
    }

    PositionalObject::PositionalObject(ObjectManager& manager) : Object(manager)
    {
        LinkAll();
        CalculateBounds();
    }

    PositionalObject::PositionalObject(const PositionalObject& arg) : Object(arg), position(arg.position)
    {
        LinkAll();
        CalculateBounds();
    }

    PositionalObject::PositionalObject(PositionalObject&& arg) : Object(std::move(arg)), position(std::move(arg.position))
    {
        LinkAll();
        CalculateBounds();
    }

    PositionalObject::PositionalObject(const ::Inscription::Table<PositionalObject>& table) :
        INSCRIPTION_TABLE_GET_BASE(Object), INSCRIPTION_TABLE_GET_MEM(position)
    {
        LinkAll();
        CalculateBounds();
    }

    PositionalObject::~PositionalObject()
    {}

    PositionalObject& PositionalObject::operator=(const PositionalObject& arg)
    {
        position = arg.position;
        return *this;
    }

    PositionalObject& PositionalObject::operator=(PositionalObject&& arg)
    {
        position = std::move(arg.position);
        return *this;
    }

    ObjectTypeDescription PositionalObject::TypeDescription() const
    {
        return ObjectTraits<PositionalObject>::TypeDescription();
    }

    void PositionalObject::CalculateBounds()
    {
        bounds.underlying.SetLeft(position.Get().GetX() - (size.Get().GetScaledWidth() / 2));
        bounds.underlying.SetTop(position.Get().GetY() - (size.Get().GetScaledHeight() / 2));
        bounds.underlying.SetFarZ(position.Get().GetZ() - (size.Get().GetScaledDepth() / 2));
        bounds.underlying.SetRight(bounds.underlying.GetLeft() + size.Get().GetScaledWidth());
        bounds.underlying.SetBottom(bounds.underlying.GetTop() + size.Get().GetScaledHeight());
        bounds.underlying.SetNearZ(bounds.underlying.GetFarZ() + size.Get().GetScaledDepth());
    }

    TypedObjectReference<PositionalObject> PositionalObject::ToReference()
    {
        return TypedObjectReference<PositionalObject>(this);
    }

    void PositionalObject::LinkAll()
    {
        position.owner = this;
        size.owner = this;
    }

    void PositionalObject::PositionHasChanged(Position3D previous)
    {
        AxisBoundingBox3D previousBounds(bounds);
        CalculateBounds();

        onPositionChanged(ToReference(), previous);
        BoundsHasChanged(previousBounds);
    }

    void PositionalObject::SizeHasChanged(Size3D previous)
    {
        AxisBoundingBox3D previousBounds(bounds);
        CalculateBounds();

        onSizeChanged(ToReference(), previous);
        BoundsHasChanged(previousBounds);
    }

    void PositionalObject::BoundsHasChanged(AxisBoundingBox3D previous)
    {
        onBoundsChanged(ToReference(), previous);
    }

    const ObjectTypeName ObjectTraits<PositionalObject>::typeName = "PositionalObject";
}

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::PositionalObject)
    {
        INSCRIPTION_TABLE_ADD(position);
        INSCRIPTION_TABLE_ADD(size);
    }

    INSCRIPTION_INSCRIPTER_DEFINE_SERIALIZE_FUNCTION(::Atmos::PositionalObject)
    {
        INSCRIPTION_INSCRIPTER_CALL_BASE_SERIALIZE_FUNCTION;
        obj.position.Calculate();
        obj.size.Calculate();
        obj.CalculateBounds();
    }
}