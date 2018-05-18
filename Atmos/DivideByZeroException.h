#pragma once

#include <exception>

namespace Atmos
{
    struct DivideByZeroException : public std::exception
    {
        DivideByZeroException();
    };
}