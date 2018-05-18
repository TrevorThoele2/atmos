#pragma once

#include "LightSource.h"

#include "Position3D.h"

namespace Atmos
{
    class PointLight : public LightSource
    {
    private:
        Position3D position;

        bool IsInBounds() const override;

        void SetPositionImpl(const Position3D &set) override;
        Optional<Position3D> GetPositionImpl() const override;
        bool HasPositionImpl() const override;
    public:
        PointLight(const Color &color, Intensity intensity, const Position3D &position);
    };
}