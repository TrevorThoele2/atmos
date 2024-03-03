#pragma once

#include "nModulatorInterpolation.h"

namespace Atmos
{
    namespace Modulator
    {
        class InterpolationSmoothstep : public Interpolation
        {
        public:
            InterpolationSmoothstep();

            InterpolationSmoothstep* Clone() const override;
            Delta TransformDelta(Delta delta) const override;
        };
    }
}