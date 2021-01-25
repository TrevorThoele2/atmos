#pragma once

#include <stdexcept>

namespace Atmos
{
    class RangeMinNotLessThanEqualMax final : public std::runtime_error
    {
    public:
        RangeMinNotLessThanEqualMax();
    };
}