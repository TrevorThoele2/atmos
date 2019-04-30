
#include "Size3D.h"

namespace Atmos
{
    Size3D::Size3D(
        ValueT width,
        ValueT height,
        ValueT depth,
        ValueT xScaler,
        ValueT yScaler,
        ValueT zScaler,
        const Angle& xRotation,
        const Angle& yRotation,
        const Angle& zRotation) :
        
        width(width), height(height), depth(depth), xScaler(xScaler), yScaler(yScaler), zScaler(zScaler),
        xRotation(xRotation), yRotation(yRotation), zRotation(zRotation)
    {
        Calculate();
    }

    Size3D::Size3D(const Size3D& arg) :
        width(arg.width), height(arg.height), depth(arg.depth),
        realWidth(arg.realWidth), realHeight(arg.realHeight), realDepth(arg.realDepth),
        xScaler(arg.xScaler), yScaler(arg.yScaler), zScaler(arg.zScaler),
        xRotation(arg.xRotation), yRotation(arg.yRotation), zRotation(arg.zRotation)
    {}

    Size3D& Size3D::operator=(const Size3D& arg)
    {
        width = arg.width;
        height = arg.height;
        depth = arg.depth;
        realWidth = arg.realWidth;
        realHeight = arg.realHeight;
        realDepth = arg.realDepth;
        xScaler = arg.xScaler;
        yScaler = arg.yScaler;
        zScaler = arg.zScaler;
        xRotation = arg.xRotation;
        yRotation = arg.yRotation;
        zRotation = arg.zRotation;
        return *this;
    }

    bool Size3D::operator==(const Size3D& arg) const
    {
        return
            width == arg.width &&
            height == arg.height &&
            depth == arg.depth &&
            realWidth == arg.realWidth &&
            realHeight == arg.realHeight &&
            realDepth == arg.realDepth &&
            xScaler == arg.xScaler &&
            yScaler == arg.yScaler &&
            zScaler == arg.zScaler &&
            xRotation == arg.xRotation &&
            yRotation == arg.yRotation &&
            zRotation == arg.zRotation;
    }

    bool Size3D::operator!=(const Size3D& arg) const
    {
        return !(*this == arg);
    }

    void Size3D::SetWidth(ValueT setTo)
    {
        width = setTo;
        Calculate();
    }

    void Size3D::SetHeight(ValueT setTo)
    {
        height = setTo;
        Calculate();
    }

    void Size3D::SetDepth(ValueT setTo)
    {
        depth = setTo;
        Calculate();
    }

    Size3D::ValueT Size3D::GetWidth() const
    {
        return width;
    }

    Size3D::ValueT Size3D::GetHeight() const
    {
        return height;
    }

    Size3D::ValueT Size3D::GetDepth() const
    {
        return depth;
    }

    Size3D::ValueT Size3D::GetScaledWidth() const
    {
        return realWidth;
    }

    Size3D::ValueT Size3D::GetScaledHeight() const
    {
        return realHeight;
    }

    Size3D::ValueT Size3D::GetScaledDepth() const
    {
        return realDepth;
    }

    void Size3D::SetXScaler(ValueT setTo)
    {
        xScaler = setTo;
        Calculate();
    }

    void Size3D::SetYScaler(ValueT setTo)
    {
        yScaler = setTo;
        Calculate();
    }

    void Size3D::SetZScaler(ValueT setTo)
    {
        zScaler = setTo;
        Calculate();
    }

    Join3<Size3D::ValueT> Size3D::GetScalers() const
    {
        return Join3<ValueT>(xScaler, yScaler, zScaler);
    }

    Size3D::ValueT Size3D::GetXScaler() const
    {
        return xScaler;
    }

    Size3D::ValueT Size3D::GetYScaler() const
    {
        return yScaler;
    }

    Size3D::ValueT Size3D::GetZScaler() const
    {
        return zScaler;
    }

    void Size3D::SetXRotation(const Angle& setTo)
    {
        xRotation = setTo;
        Calculate();
    }

    void Size3D::SetYRotation(const Angle& setTo)
    {
        yRotation = setTo;
        Calculate();
    }

    void Size3D::SetZRotation(const Angle& setTo)
    {
        zRotation = setTo;
        Calculate();
    }

    const Angle& Size3D::GetXRotation() const
    {
        return xRotation;
    }

    const Angle& Size3D::GetYRotation() const
    {
        return yRotation;
    }

    const Angle& Size3D::GetZRotation() const
    {
        return zRotation;
    }

    void Size3D::Calculate()
    {
        auto sinT = sin(xRotation.As<Radians>());
        auto cosT = cos(xRotation.As<Radians>());

        realWidth = width * xScaler;
        realHeight = height * yScaler;
        realDepth = depth * zScaler;

        auto prevWidth = realWidth;
        auto prevHeight = realHeight;
        auto prevDepth = realDepth;

        realWidth = (prevHeight * sinT) + (prevWidth * cosT);
        realHeight = (prevWidth * sinT) + (prevHeight * cosT);
    }

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Size3D)
    {
        scribe(width);
        scribe(height);
        scribe(depth);
        scribe(xScaler);
        scribe(yScaler);
        scribe(zScaler);
        scribe(xRotation);
        scribe(yRotation);
        scribe(zRotation);
        if (scribe.IsInput())
            Calculate();
    }
}