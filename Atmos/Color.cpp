
#include "Color.h"

#include <Inscription\Scribe.h>

namespace Atmos
{
    INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DEFINE(Color)
    {
        scribe(alpha);
        scribe(red);
        scribe(green);
        scribe(blue);
    }

    Color::Color(ValueT alpha, ValueT red, ValueT green, ValueT blue) : alpha(alpha), red(red), green(green), blue(blue)
    {}

    Color::Color(const Color &arg) : alpha(arg.alpha), red(arg.red), green(arg.green), blue(arg.blue)
    {}

    Color& Color::operator=(const Color& arg)
    {
        alpha = arg.alpha;
        red = arg.red;
        green = arg.green;
        blue = arg.blue;
        return *this;
    }

    bool Color::operator==(const Color& arg) const
    {
        return alpha == arg.alpha && red == arg.red && green == arg.green && blue == arg.blue;
    }

    bool Color::operator!=(const Color &arg) const
    {
        return !(*this == arg);
    }

    void Color::Edit(ValueT red, ValueT green, ValueT blue)
    {
        this->red = red;
        this->green = green;
        this->blue = blue;
    }

    void Color::Edit(ValueT alpha, ValueT red, ValueT green, ValueT blue)
    {
        this->alpha = alpha;
        this->red = alpha;
        this->green = green;
        this->blue = blue;
    }

    void Color::SetAlpha(ValueT setTo)
    {
        alpha = setTo;
    }

    void Color::SetRed(ValueT setTo)
    {
        red = setTo;
    }

    void Color::SetGreen(ValueT setTo)
    {
        green = setTo;
    }

    void Color::SetBlue(ValueT setTo)
    {
        blue = setTo;
    }

    Color::ValueT Color::GetAlpha() const
    {
        return alpha;
    }

    Color::ValueT Color::GetRed() const
    {
        return red;
    }

    Color::ValueT Color::GetGreen() const
    {
        return green;
    }

    Color::ValueT Color::GetBlue() const
    {
        return blue;
    }
}