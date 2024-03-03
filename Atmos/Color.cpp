#include "Color.h"

#include <Inscription/Scribe.h>

namespace Atmos::Render
{
    Color::Color() : alpha(255), red(255), green(255), blue(255)
    {}

    Color::Color(Value alpha, Value red, Value green, Value blue)
        : alpha(alpha), red(red), green(green), blue(blue)
    {}

    bool Color::operator==(const Color& arg) const
    {
        return alpha == arg.alpha && red == arg.red && green == arg.green && blue == arg.blue;
    }

    bool Color::operator!=(const Color& arg) const
    {
        return !(*this == arg);
    }
}