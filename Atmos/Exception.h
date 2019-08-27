#pragma once

#include <exception>
#include "String.h"

namespace Atmos
{
    class Exception : public std::exception
    {
    public:
        explicit Exception(const String& message);
    };
}