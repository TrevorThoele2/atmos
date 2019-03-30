#pragma once

#include <stdexcept>
#include "String.h"

namespace Atmos
{
    class NotImplementedException : public std::runtime_error
    {
    public:
        NotImplementedException(const String& message);
    };
}