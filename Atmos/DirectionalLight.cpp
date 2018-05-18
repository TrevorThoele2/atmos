
#include "DirectionalLight.h"

namespace Atmos
{
    void DirectionalLight::Draw(Canvas &lightMap)
    {

    }

    DirectionalLight::DirectionalLight(const Color &color, Intensity intensity, const Direction &direction) : LightSource(color, intensity), direction(direction)
    {}
}