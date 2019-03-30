#pragma once

#include "nModulatorInterpolation.h"

namespace Atmos
{
    namespace Modulator
    {
        class InterpolationOscillation : public Interpolation
        {
        public:
            typedef float Acceleration;
            typedef float Rate;
        public:
            Acceleration acceleration;
            Rate rate;
        public:
            InterpolationOscillation();

            InterpolationOscillation* Clone() const override;
            Delta TransformDelta(Delta delta) const override;
        };
    }
}