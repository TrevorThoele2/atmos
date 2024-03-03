
#pragma once

#include "TimerBase.h"

#include "Serialization.h"

namespace Atmos
{
    class RealTimer : public TimerBase
    {
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    public:
        RealTimer(TimeValue goal = TimeValue());
        bool operator==(const RealTimer &arg) const;
        TimeValue CurrentTime() const override;
    };
}