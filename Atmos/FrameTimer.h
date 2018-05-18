
#pragma once

#include "TimerBase.h"
#include "Serialization.h"

namespace Atmos
{
    class FrameTimer : public TimerBase
    {
    public:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    public:
        FrameTimer(TimeValue goal = TimeValue());
        bool operator==(const FrameTimer &arg) const;
        bool operator!=(const FrameTimer &arg) const;
        TimeValue CurrentTime() const override;
    };
}