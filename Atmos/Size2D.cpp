
#include <math.h>

#include "Size2D.h"

#include <Inscription\Scribe.h>

namespace Atmos
{
    Size2D::Size2D(ValueT width, ValueT height, ValueT xScaler, ValueT yScaler, const Angle& rotation) :
        width(width), height(height),
        xScaler(xScaler), yScaler(yScaler),
        rotation(rotation)
    {
        Calculate();
    }

    Size2D::Size2D(const Size2D& arg) :
        width(arg.width), height(arg.height),
        realWidth(arg.realWidth), realHeight(arg.realHeight),
        xScaler(arg.xScaler), yScaler(arg.yScaler),
        rotation(arg.rotation)
    {}

    Size2D& Size2D::operator=(const Size2D& arg)
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

    bool Size2D::operator==(const Size2D& arg) const
    {
        return
            width == arg.width &&
            height == arg.height &&
            realWidth == arg.realWidth &&
            realHeight == arg.realHeight &&
            xScaler == arg.xScaler &&
            yScaler == arg.yScaler &&
            rotation == arg.rotation;
    }

    bool Size2D::operator!=(const Size2D& arg) const
    {
        return !(*this == arg);
    }

    void Size2D::SetWidth(ValueT setTo)
    {
        width = setTo;
        Calculate();
    }

    void Size2D::SetHeight(ValueT setTo)
    {
        height = setTo;
        Calculate();
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
        Calculate();
    }

    void Size2D::SetYScaler(ValueT setTo)
    {
        yScaler = setTo;
        Calculate();
    }

    Size2D::Scalers Size2D::GetScalers() const
    {
        Scalers scalers;
        scalers.x = xScaler;
        scalers.y = yScaler;
        return scalers;
    }

    Size2D::ValueT Size2D::GetXScaler() const
    {
        return xScaler;
    }

    Size2D::ValueT Size2D::GetYScaler() const
    {
        return yScaler;
    }

    void Size2D::SetRotation(const Angle& setTo)
    {
        rotation = setTo;
        Calculate();
    }

    const Angle& Size2D::GetRotation() const
    {
        return rotation;
    }

    void Size2D::Calculate()
    {
        auto sinValue = sin(rotation.As<Radians>());
        auto cosValue = cos(rotation.As<Radians>());

        realWidth = width * xScaler;
        realHeight = height * yScaler;

        auto prevWidth = realWidth;
        auto prevHeight = realHeight;

        realWidth = (prevHeight * sinValue) + (prevWidth * cosValue);
        realHeight = (prevWidth * sinValue) + (prevHeight * cosValue);
    }

    INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DEFINE(Size2D)
    {
        scribe(width);
        scribe(height);
        scribe(xScaler);
        scribe(yScaler);
        scribe(rotation);
        if (scribe.IsInput())
            Calculate();
    }
}