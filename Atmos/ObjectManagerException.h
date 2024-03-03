#pragma once

#include <exception>
#include "String.h"

namespace Atmos
{
    class ObjectManagerException : public std::runtime_error
    {
    public:
        ObjectManagerException(const String& message);
    };

    class IncorrectRegisteredObjectSystemTypeException : public ObjectManagerException
    {
    public:
        IncorrectRegisteredObjectSystemTypeException();
    };
}