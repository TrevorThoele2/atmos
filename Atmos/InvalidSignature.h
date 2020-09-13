#pragma once

#include "Exception.h"

namespace Atmos
{
    class InvalidSignature final : public Exception
    {
    public:
        InvalidSignature();
    };
}