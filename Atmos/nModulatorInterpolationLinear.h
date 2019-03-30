#pragma once

#include "nModulatorInterpolation.h"

namespace Atmos
{
    namespace Modulator
    {
        class InterpolationLinear : public Interpolation
        {
        public:
            InterpolationLinear();

            InterpolationLinear* Clone() const override;
            Delta TransformDelta(Delta delta) const override;
        };
    }
}