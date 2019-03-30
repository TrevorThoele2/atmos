#pragma once

#include "TimeValue.h"

namespace Atmos
{
    namespace Modulator
    {
        class Interpolation
        {
        public:
            enum class Type
            {
                NONE,

                LINEAR,
                OSCILLATION,
                SMOOTHSTEP
            };

            typedef TimeValue::ValueT Delta;
        public:
            const Type type;
        public:
            virtual Interpolation* Clone() const = 0;
            virtual Delta TransformDelta(Delta delta) const = 0;
        protected:
            Interpolation(Type type);
        };
    }
}