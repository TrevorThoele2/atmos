#pragma once

#include "LightSource.h"

#include "Position2D.h"

namespace Atmos
{
    class Spotlight : public LightSource
    {
    private:
        Position2D position;
    public:
        Spotlight(const Color &color, Intensity intensity, const Position2D &position);
    };
}