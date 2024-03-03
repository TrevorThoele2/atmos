#include "Color.h"

namespace Atmos::Render
{
    bool Color::operator==(const Color& arg) const
    {
        return alpha == arg.alpha && red == arg.red && green == arg.green && blue == arg.blue;
    }

    bool Color::operator!=(const Color& arg) const
    {
        return !(*this == arg);
    }
}