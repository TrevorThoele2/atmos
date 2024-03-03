#pragma once

#include <exception>

namespace Atmos
{
    struct InputException : public std::exception
    {
        InputException();
    };
}