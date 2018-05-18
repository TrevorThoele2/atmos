#pragma once

#include "Overflow.h"

namespace Atmos
{
    template<class T>
    // Left MUST be within the range
    T WrapAdd(T left, T right, T low, T high)
    {
        right %= high - low;
        if (OverflowAdd(left, right, high))
            return (left - low) + right + 1;
        else
            return left + right;
    }

    template<class T>
    // Left MUST be within the range
    T WrapSubtract(T left, T right, T low, T high)
    {
        right %= high - low;
        if (UnderflowSubtract(left, right, low))
            return (high - right) + (left - low) + 1;
        else
            return left - right;
    }
}