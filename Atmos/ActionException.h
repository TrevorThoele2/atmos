#pragma once

#include <exception>

namespace Atmos
{
    namespace Act
    {
        class ActionException : public std::exception
        {
        public:
            ActionException(const char* message) : exception(message)
            {}
        };
    }
}