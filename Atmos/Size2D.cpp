
#include <math.h>

#include "Size2D.h"

#include <Inscription\Scribe.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Size2D)
    {
        scribe(width, height);
        scribe(xScaler, yScaler);
        scribe(rotation);
        if (scribe.IsInput())
            Calc();
    }

    void Size2D::Calc()
    {
        auto sinT = sin(rotation.value);
        auto cosT = cos(rotation.value);

        realWidth = width * xScaler;
        realHeight = height * yScaler;

        auto prevWidth = realWidth;
        auto prevHeight = realHeight;

        realWidth = (prevHeight * sinT) + (prevWidth * cosT);
        realHeight = (prevWidth * sinT) + (prevHeight * cosT);
    }

    Size2D::Size2D(ValueT width, ValueT height, ValueT xScaler, ValueT yScaler, const Angle &rotation) : width(width), height(height), xScaler(xScaler), yScaler(yScaler), rotation(rotation)
    {
        Calc();
    }

    Size2D::Size2D(const Size2D &arg) : width(arg.width), height(arg.height), realWidth(arg.realWidth), realHeight(arg.realHeight), xScaler(arg.xScaler), yScaler(arg.yScaler), rotation(arg.rotation)
    {}

    Size2D& Size2D::operator=(const Size2D &arg)
    {
        width = arg.width;
        height = arg.height;
        realWidth = arg.realWidth;
        realHeight = arg.realHeight;
        xScaler = arg.xScaler;
        yScaler = arg.yScaler;
        rotation = arg.rotation;
        return *this;
    }

    bool Size2D::operator==(const Size2D &arg) const
    {
        return width == arg.width && height == arg.height && realWidth == arg.realWidth && realHeight == arg.realHeight && xScaler == arg.xScaler && yScaler == arg.yScaler && rotation == arg.rotation;
    }

    bool Size2D::operator!=(const Size2D &arg) const
    {
        return !(*this == arg);
    }

    void Size2D::SetWidth(ValueT setTo)
    {
        width = setTo;
        Calc();
    }

    void Size2D::SetHeight(ValueT setTo)
    {
        height = setTo;
        Calc();
    }

    Size2D::ValueT Size2D::GetWidth() const
    {
        return width;
    }

    Size2D::ValueT Size2D::GetHeight() const
    {
        return height;
    }

    Size2D::ValueT Size2D::GetScaledWidth() const
    {
        return realWidth;
    }

    Size2D::ValueT Size2D::GetScaledHeight() const
    {
        return realHeight;
    }

    void Size2D::SetXScaler(ValueT setTo)
    {
        xScaler = setTo;
        Calc();
    }

    void Size2D::SetYScaler(ValueT setTo)
    {
        yScaler = setTo;
        Calc();
    }

    Join2<Size2D::ValueT> Size2D::GetScalers() const
    {
        return Join2<ValueT>(xScaler, yScaler);
    }

    Size2D::ValueT Size2D::GetXScaler() const
    {
        return xScaler;
    }

    Size2D::ValueT Size2D::GetYScaler() const
    {
        return yScaler;
    }

    void Size2D::SetRotation(const Angle &setTo)
    {
        rotation = setTo;
        Calc();
    }

    const Angle& Size2D::GetRotation() const
    {
        return rotation;
    }
}