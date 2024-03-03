#pragma once

#include "Exception.h"

namespace Atmos
{
    class DivisionByZero final : public Exception
    {
    public:
        DivisionByZero();
    };
}