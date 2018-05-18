#pragma once

#include "LightSource.h"

#include "Direction.h"

namespace Atmos
{
    class DirectionalLight : public LightSource
    {
    private:
        Direction direction;

        void Draw(Canvas &lightMap);
    public:
        DirectionalLight(const Color &color, Intensity intensity, const Direction &direction);
    };
}