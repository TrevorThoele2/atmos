#pragma once

#include <stdexcept>

namespace Atmos
{
    class ObjectBatchInvalidatedException : public std::runtime_error
    {
    public:
        ObjectBatchInvalidatedException();
    };
}