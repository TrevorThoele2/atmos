
#include "Spotlight.h"

namespace Atmos
{
    Spotlight::Spotlight(const Color &color, Intensity intensity, const Position2D &position) : LightSource(color, intensity), position(position)
    {}
}