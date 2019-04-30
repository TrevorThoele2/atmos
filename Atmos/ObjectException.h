#pragma once

#include <exception>
#include "String.h"

namespace Atmos
{
    class ObjectException : public std::runtime_error
    {
    public:
        ObjectException(const String& message);
    };
}