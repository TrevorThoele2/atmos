#pragma once

/*
#include "TimeValue.h"

namespace Atmos
{
    namespace Modulator
    {
        class Interpolation
        {
        public:
            enum class Severity
            {
                NONE,

                LINEAR,
                OSCILLATION,
                SMOOTHSTEP
            };

            typedef TimeValue::ValueT Delta;
        public:
            const Severity type;
        public:
            virtual Interpolation* Clone() const = 0;
            virtual Delta TransformDelta(Delta delta) const = 0;
        protected:
            Interpolation(Severity type);
        };
    }
}
*/