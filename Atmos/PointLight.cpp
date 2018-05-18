
#include "PointLight.h"

#include "Camera.h"
#include "Position2D.h"
#include "AxisBoundingBox2D.h"

namespace Atmos
{
    bool PointLight::IsInBounds() const
    {
        const Intensity halfIntensity(GetIntensity() / 2.0f);
        AxisBoundingBox2D box(position.GetX() - halfIntensity, position.GetY() - halfIntensity, position.GetX() + halfIntensity, position.GetY() + halfIntensity);

        return box.Within(Camera::GetSides());
    }

    void PointLight::SetPositionImpl(const Position3D &set)
    {
        position = set;
    }

    Optional<Position3D> PointLight::GetPositionImpl() const
    {
        return Optional<Position3D>(position);
    }

    bool PointLight::HasPositionImpl() const
    {
        return true;
    }

    PointLight::PointLight(const Color &color, Intensity intensity, const Position3D &position) : LightSource(color, intensity), position(position)
    {}
}