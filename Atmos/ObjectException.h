#pragma once

#include <stdexcept>
#include "String.h"

namespace Atmos
{
    class ObjectException : public std::runtime_error
    {
    public:
        ObjectException(const String& message);
    };

    class ObjectNonexistantException : public ObjectException
    {
    public:
        ObjectNonexistantException();
    };
}